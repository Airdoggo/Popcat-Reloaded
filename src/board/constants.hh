#pragma once

typedef unsigned long long Bitboard;

namespace board
{
    static constexpr const Bitboard RANK3OR6 = 0x0000FF0000FF0000;
    static constexpr const Bitboard RANK1OR8 = 0xFF000000000000FF;
    static constexpr const Bitboard RANK4 = 0x00000000FF000000;
    static constexpr const Bitboard RANK5 = 0x000000FF00000000;
    static constexpr const Bitboard RANK4OR5 = RANK4 | RANK5;
    static constexpr const Bitboard NOTFILEA = 0xFEFEFEFEFEFEFEFE;
    static constexpr const Bitboard NOTFILEH = 0x7F7F7F7F7F7F7F7F;

    static constexpr const Bitboard WQ_CASTLING = 0x000000000000000E;
    static constexpr const Bitboard WK_CASTLING = 0x0000000000000060;
    static constexpr const Bitboard BQ_CASTLING = 0x0E00000000000000;
    static constexpr const Bitboard BK_CASTLING = 0x6000000000000000;
    static constexpr const Bitboard Q_CASTLING = WQ_CASTLING | BQ_CASTLING;
    static constexpr const Bitboard K_CASTLING = WK_CASTLING | BK_CASTLING;

    static constexpr const Bitboard WRQ_CASTLING_START_POSITION = 0x0000000000000001;
    static constexpr const Bitboard WRK_CASTLING_START_POSITION = 0x0000000000000080;
    static constexpr const Bitboard BRQ_CASTLING_START_POSITION = 0x0100000000000000;
    static constexpr const Bitboard BRK_CASTLING_START_POSITION = 0x8000000000000000;
    static constexpr const Bitboard WR_STARTING_POSITION = WRQ_CASTLING_START_POSITION | WRK_CASTLING_START_POSITION;
    static constexpr const Bitboard BR_STARTING_POSITION = BRQ_CASTLING_START_POSITION | BRK_CASTLING_START_POSITION;
    static constexpr const Bitboard RQ_CASTLING_START_POSITION =
        WRQ_CASTLING_START_POSITION | BRQ_CASTLING_START_POSITION;
    static constexpr const Bitboard RK_CASTLING_START_POSITION =
        WRK_CASTLING_START_POSITION | BRK_CASTLING_START_POSITION;

    static constexpr const Bitboard WQ_CASTLING_CHECK = 0x000000000000000C;
    static constexpr const Bitboard BQ_CASTLING_CHECK = 0x0C00000000000000;

    static constexpr const Bitboard WQ_CASTLING_ROOK_MOVE = 0x0000000000000009;
    static constexpr const Bitboard WK_CASTLING_ROOK_MOVE = 0x000000000000000A0;
    static constexpr const Bitboard BQ_CASTLING_ROOK_MOVE = 0x0900000000000000;
    static constexpr const Bitboard BK_CASTLING_ROOK_MOVE = 0xA000000000000000;
} // namespace board