#include "chessboard.hh"

namespace board
{
    bool Chessboard::validate_move(Bitboard *moving_piece, Bitboard *target, Bitboard move_start, Bitboard move_end)
    {
        ColorBitboards *color = _white_turn ? colors[0] : colors[1];
        Bitboard *friend_pieces = color->friends;
        Bitboard *enemy_pieces = color->enemies;
        Bitboard total_move = move_start | move_end;

        *moving_piece ^= total_move;
        *friend_pieces ^= total_move;

        if (target != nullptr)
        {
            *target ^= move_end;
            *enemy_pieces ^= move_end;
        }

        Bitboard attack_board = get_attack_board();
        bool result = attack_board & color->king;

        *moving_piece ^= total_move;
        *friend_pieces ^= total_move;

        if (target != nullptr)
        {
            *target ^= move_end;
            *enemy_pieces ^= move_end;
        }

        return !result;
    }

    bool Chessboard::is_in_check()
    {
        Bitboard attack_board = get_attack_board();
        return attack_board & (_white_turn ? _white_bitboards.king : _black_bitboards.king);
    }

    inline static Bitboard get_pawn_attacks(Bitboard pawns, bool white_pawns)
    {
        return (((white_pawns ? pawns << 9 : pawns >> 7) & NOTFILEA)
                | ((white_pawns ? pawns << 7 : pawns >> 9) & NOTFILEH));
    }

    inline static Bitboard get_knight_attacks(Chessboard *board, Bitboard knights)
    {
        unsigned long index;
        Bitboard result = 0x0;

        while (_BitScanForward64(&index, knights))
        {
            result |= board->tables.knight[index];
            knights ^= 1ULL << index;
        }

        return result;
    }

    inline static Bitboard get_king_attacks(Chessboard *board, Bitboard king)
    {
        unsigned long index;
        _BitScanForward64(&index, king);
        return board->tables.king[index];
    }

    inline static Bitboard get_sliding_attacks(Chessboard *board, Bitboard pieces, Bitboard blockers,
                                               SlidingPieceType type)
    {
        unsigned long index;
        Bitboard result = 0x0;

        while (_BitScanForward64(&index, pieces))
        {
            if (type & SlidingPieceType::ROOK)
            {
                Bitboard masked_blockers = board->tables.rook_masks[index] & blockers;
                Bitboard magic_number = board->tables.rook_magic_number[index];
                Bitboard key = ((masked_blockers * magic_number) >> (64 - board->tables.rook_magic_bits[index]));

                result |= board->tables.rook_attack_table[index][key];
            }
            if (type & SlidingPieceType::BISHOP)
            {
                Bitboard masked_blockers = board->tables.bishop_masks[index] & blockers;
                Bitboard magic_number = board->tables.bishop_magic_number[index];
                Bitboard key = ((masked_blockers * magic_number) >> (64 - board->tables.bishop_magic_bits[index]));

                result |= board->tables.bishop_attack_table[index][key];
            }

            pieces ^= 1ULL << index;
        }

        return result;
    }

    Bitboard Chessboard::get_attack_board()
    {
        ColorBitboards *color = _white_turn ? colors[1] : colors[0];
        Bitboard blockers = _whites | _blacks;

        return get_pawn_attacks(color->pawns, !_white_turn) | get_knight_attacks(this, color->knights)
            | get_king_attacks(this, color->king)
            | get_sliding_attacks(this, color->rooks, blockers, SlidingPieceType::ROOK)
            | get_sliding_attacks(this, color->bishops, blockers, SlidingPieceType::BISHOP)
            | get_sliding_attacks(this, color->queen, blockers, SlidingPieceType::QUEEN);
    }
} // namespace board