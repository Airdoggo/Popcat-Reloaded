#pragma once

typedef unsigned long long Bitboard;

namespace board
{
    enum MoveType
    {
        PROMOTION_QUEEN,
        PROMOTION_ROOK,
        PROMOTION_BISHOP,
        PROMOTION_KNIGHT,
        CASTLING_KING,
        CASTLING_QUEEN,
        BREAK_CASTLING_KING,
        BREAK_BOTH_CASTLING,
        BREAK_CASTLING_QUEEN,
        PASSING,
        EN_PASSANT,
        NONE,
    };

    struct Move final
    {
        Bitboard bitboard_move;

        Bitboard *piece_board;
        Bitboard *target_board;

        bool is_white;
        MoveType type = MoveType::NONE;

        int move_score = 0;
    };
} // namespace board
