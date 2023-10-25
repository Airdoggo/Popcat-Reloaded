#pragma once

typedef unsigned long long Bitboard;

#include <string>

namespace board
{
    class Chessboard final
    {
    public:
        Chessboard();
        Chessboard(std::string fen_string);

        void pretty_print() const;
        void print_bitboard(Bitboard bitboard) const;

    private:
        unsigned _turn = 0;
        unsigned _halfmoves = 0;

        bool _white_turn = true;
        bool _white_king_castling = false;
        bool _white_queen_castling = false;
        bool _black_king_castling = false;
        bool _black_queen_castling = false;

        Bitboard _whites = 0;
        Bitboard _blacks = 0;

        Bitboard _white_bishops = 0;
        Bitboard _white_king = 0;
        Bitboard _white_knights = 0;
        Bitboard _white_pawns = 0;
        Bitboard _white_queen = 0;
        Bitboard _white_rooks = 0;

        Bitboard _black_bishops = 0;
        Bitboard _black_king = 0;
        Bitboard _black_knights = 0;
        Bitboard _black_pawns = 0;
        Bitboard _black_queen = 0;
        Bitboard _black_rooks = 0;

        Bitboard _en_passant = 0;
    };
} // namespace board
