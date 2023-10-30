#include "chessboard.hh"

namespace board
{
    bool Chessboard::validate_move(Bitboard *moving_piece, Bitboard *target, Bitboard move_start, Bitboard move_end)
    {
        Bitboard *friend_pieces = _white_turn ? &_whites : &_blacks;
        Bitboard *enemy_pieces = _white_turn ? &_blacks : &_whites;
        Bitboard total_move = move_start | move_end;

        *moving_piece ^= total_move;
        *friend_pieces ^= total_move;

        if (target != nullptr)
        {
            *target ^= move_end;
            *enemy_pieces ^= move_end;
        }

        Bitboard attack_board = get_attack_board();
        bool result = attack_board & (_white_turn ? _white_king : _black_king);

        *moving_piece ^= total_move;
        *friend_pieces ^= total_move;

        if (target != nullptr)
        {
            *target ^= move_end;
            *enemy_pieces ^= move_end;
        }

        return !result;
    }

    static constexpr Bitboard NOTFILEA = 0xFEFEFEFEFEFEFEFE;
    static constexpr Bitboard NOTFILEH = 0x7F7F7F7F7F7F7F7F;

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

    inline static Bitboard get_sliding_attacks(Chessboard *board, Bitboard pieces, Bitboard blockers, PieceType type)
    {
        unsigned long index;
        Bitboard result = 0x0;

        while (_BitScanForward64(&index, pieces))
        {
            if (type & PieceType::ROOK)
            {
                Bitboard masked_blockers = board->tables.rook_masks[index] & blockers;
                Bitboard magic_number = board->tables.rook_magic_number[index];
                Bitboard key = ((masked_blockers * magic_number) >> (64 - board->tables.rook_magic_bits[index]));

                result |= board->tables.rook_attack_table[index][key];
            }
            if (type & PieceType::BISHOP)
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
        Bitboard blockers = _whites | _blacks;

        if (_white_turn)
        {
            return get_pawn_attacks(_black_pawns, false) | get_knight_attacks(this, _black_knights)
                | get_king_attacks(this, _black_king)
                | get_sliding_attacks(this, _black_rooks, blockers, PieceType::ROOK)
                | get_sliding_attacks(this, _black_bishops, blockers, PieceType::BISHOP)
                | get_sliding_attacks(this, _black_queen, blockers, PieceType::QUEEN);
        }

        return get_pawn_attacks(_white_pawns, true) | get_knight_attacks(this, _white_knights)
            | get_king_attacks(this, _white_king) | get_sliding_attacks(this, _white_rooks, blockers, PieceType::ROOK)
            | get_sliding_attacks(this, _white_bishops, blockers, PieceType::BISHOP)
            | get_sliding_attacks(this, _white_queen, blockers, PieceType::QUEEN);
    }
} // namespace board