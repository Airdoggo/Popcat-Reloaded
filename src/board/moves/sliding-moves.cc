#include "chessboard.hh"

namespace board
{
    void Chessboard::generate_sliding_moves(std::vector<Move> &moves, SlidingPieceType type)
    {
        ColorBitboards *color = white_turn ? &white_bitboards : &black_bitboards;
        Bitboard *friendly_pieces;
        switch (type)
        {
        case BISHOP:
            friendly_pieces = &color->bishops;
            break;
        case ROOK:
            friendly_pieces = &color->rooks;
            break;
        default:
            friendly_pieces = &color->queen;
            break;
        }

        Bitboard pieces = *friendly_pieces;
        Bitboard blockers = _whites | _blacks;
        Bitboard movable = ~(*color->friends);
        Bitboard enemies = *color->enemies;
        unsigned long index;

        while (_BitScanForward64(&index, pieces))
        {
            Bitboard position = 1ULL << index;
            Bitboard moves_table = 0x0;

            if (type & SlidingPieceType::ROOK)
            {
                Bitboard masked_blockers = tables::rook_masks[index] & blockers;
                Bitboard magic_number = tables::rook_magic_number[index];
                Bitboard key = ((masked_blockers * magic_number) >> (64 - tables::rook_magic_bits[index]));

                moves_table |= tables::rook_attack_table[index][key];
            }

            if (type & SlidingPieceType::BISHOP)
            {
                Bitboard masked_blockers = tables::bishop_masks[index] & blockers;
                Bitboard magic_number = tables::bishop_magic_number[index];
                Bitboard key = ((masked_blockers * magic_number) >> (64 - tables::bishop_magic_bits[index]));

                moves_table |= tables::bishop_attack_table[index][key];
            }

            moves_table &= movable;

            MoveType move_type = MoveType::NONE;

            if (type == SlidingPieceType::ROOK)
            {
                Bitboard castling = color->castling;
                if (castling)
                {
                    if (castling & Q_CASTLING
                        && position & (white_turn ? WRQ_CASTLING_START_POSITION : BRQ_CASTLING_START_POSITION))
                        move_type = MoveType::BREAK_CASTLING_QUEEN;
                    else if (castling & K_CASTLING
                             && position & (white_turn ? WRK_CASTLING_START_POSITION : BRK_CASTLING_START_POSITION))
                        move_type = MoveType::BREAK_CASTLING_KING;
                }
            }

            while (_BitScanForward64(&index, moves_table))
            {
                Bitboard move = 1ULL << index;
                Bitboard *enemy = (move & enemies) ? get_board_at_position(move, !white_turn) : nullptr;

                if (validate_move(friendly_pieces, enemy, position, move))
                    moves.push_back({ move | position, friendly_pieces, enemy, _en_passant, white_turn, move_type });

                moves_table ^= move;
            }

            pieces ^= position;
        }
    }
} // namespace board
