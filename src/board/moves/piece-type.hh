#pragma once

namespace board
{
    enum PieceType
    {
        PAWN = 0b1,
        ROOK = 0b10,
        KNIGHT = 0b100,
        BISHOP = 0b1000,
        QUEEN = ROOK | BISHOP,
        KING = 0b10000,
    };
}