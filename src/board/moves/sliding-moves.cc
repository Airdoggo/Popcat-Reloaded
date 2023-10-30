#include "chessboard.hh"

namespace board
{
    static constexpr Bitboard QUEEN_CASTLING = 0x0E0000000000000E;
    static constexpr Bitboard KING_CASTLING = 0x6000000000000060;
    static constexpr Bitboard ROOKS_QUEEN_CASTLING_START_POSITION = 0x0100000000000001;
    static constexpr Bitboard ROOKS_KING_CASTLING_START_POSITION = 0x8000000000000080;

    void Chessboard::generate_sliding_moves(std::vector<Move> &moves, PieceType type)
    {
        Bitboard *friendly_pieces;
        switch (type)
        {
        case BISHOP:
            friendly_pieces = _white_turn ? &_white_bishops : &_black_bishops;
            break;
        case ROOK:
            friendly_pieces = _white_turn ? &_white_rooks : &_black_rooks;
            break;
        case QUEEN:
            friendly_pieces = _white_turn ? &_white_queen : &_black_queen;
            break;
        default:
            return;
        }

        Bitboard pieces = *friendly_pieces;
        Bitboard blockers = _whites | _blacks;
        Bitboard movable = _white_turn ? ~_whites : ~_blacks;
        Bitboard enemies = _white_turn ? _blacks : _whites;
        unsigned long index;

        while (_BitScanForward64(&index, pieces))
        {
            Bitboard position = 1ULL << index;
            Bitboard moves_table = 0x0;

            if (type & ROOK)
            {
                Bitboard masked_blockers = tables.rook_masks[index] & blockers;
                Bitboard magic_number = tables.rook_magic_number[index];
                Bitboard key = ((masked_blockers * magic_number) >> (64 - tables.rook_magic_bits[index]));

                moves_table |= tables.rook_attack_table[index][key];
            }

            if (type & BISHOP)
            {
                Bitboard masked_blockers = tables.bishop_masks[index] & blockers;
                Bitboard magic_number = tables.bishop_magic_number[index];
                Bitboard key = ((masked_blockers * magic_number) >> (64 - tables.bishop_magic_bits[index]));

                moves_table |= tables.bishop_attack_table[index][key];
            }

            moves_table &= movable;

            MoveType move_type = MoveType::NONE;

            if (type == PieceType::ROOK)
            {
                Bitboard castling = _white_turn ? _white_castling : _black_castling;
                if (castling)
                {
                    if (castling & QUEEN_CASTLING && position & ROOKS_QUEEN_CASTLING_START_POSITION)
                        move_type = MoveType::BREAK_CASTLING_QUEEN;
                    else if (castling & KING_CASTLING && position & ROOKS_KING_CASTLING_START_POSITION)
                        move_type = MoveType::BREAK_CASTLING_KING;
                }
            }

            while (_BitScanForward64(&index, moves_table))
            {
                Bitboard move = 1ULL << index;
                Bitboard *enemy = (move & enemies) ? get_board_at_position(move, !_white_turn) : nullptr;

                if (validate_move(friendly_pieces, enemy, position, move))
                    moves.push_back({ move | position, friendly_pieces, enemy, _en_passant, _white_turn, move_type });

                moves_table ^= move;
            }

            pieces ^= position;
        }
    }
} // namespace board
