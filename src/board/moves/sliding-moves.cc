#include "chessboard.hh"

namespace board
{
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
                Bitboard masked_blockers = _tables.rook_masks[index] & blockers;
                Bitboard magic_number = _tables.rook_magic_number[index];
                Bitboard key = ((masked_blockers * magic_number) >> (64 - _tables.rook_magic_bits[index]));

                moves_table |= _tables.rook_attack_table[index][key];
            }

            if (type & BISHOP)
            {
                Bitboard masked_blockers = _tables.bishop_masks[index] & blockers;
                Bitboard magic_number = _tables.bishop_magic_number[index];
                Bitboard key = ((masked_blockers * magic_number) >> (64 - _tables.bishop_magic_bits[index]));

                moves_table |= _tables.bishop_attack_table[index][key];
            }

            moves_table &= movable;

            while (_BitScanForward64(&index, moves_table))
            {
                Bitboard move = 1ULL << index;
                Bitboard *enemy = (move & enemies) ? get_board_at_position(move, !_white_turn) : nullptr;

                moves.push_back({ move | position, friendly_pieces, enemy, _white_turn, MoveType::NONE });

                moves_table ^= move;
            }

            pieces ^= position;
        }
    }
} // namespace board
