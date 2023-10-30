#pragma once

typedef unsigned long long Bitboard;

namespace board
{
    enum MoveType
    {
        NONE = 0,

        PROMOTION_QUEEN = 0x1,
        PROMOTION_ROOK = 0x2,
        PROMOTION_BISHOP = 0x4,
        PROMOTION_KNIGHT = 0x8,
        PROMOTION = PROMOTION_QUEEN | PROMOTION_ROOK | PROMOTION_KNIGHT | PROMOTION_BISHOP,

        CASTLING_KING = 0x10,
        CASTLING_QUEEN = 0x20,
        CASTLING = CASTLING_KING | CASTLING_QUEEN,

        BREAK_CASTLING_KING = 0x40,
        BREAK_CASTLING_QUEEN = 0x80,
        BREAK_BOTH_CASTLING = BREAK_CASTLING_KING | BREAK_CASTLING_QUEEN,

        PASSING = 0x100,
        EN_PASSANT = 0x200,
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
