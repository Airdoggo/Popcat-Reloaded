#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "moves/move.hh"
#include "moves/piece-type.hh"
#include "moves/tables.hh"

typedef unsigned long long Bitboard;

namespace board
{
    class Chessboard final
    {
    public:
        Tables tables;

        Chessboard();
        Chessboard(const std::string &fen_string);

        void pretty_print() const;
        void print_bitboard(Bitboard bitboard) const;

        void generate_legal_moves(std::vector<Move> &moves);
        void do_move(const Move &move);
        void switch_turn();

        Bitboard *get_board_at_position(Bitboard position, bool is_white);

        bool validate_move(Bitboard *moving_piece, Bitboard *target, Bitboard move_start, Bitboard move_end);

    private:
        unsigned _turn = 0;
        unsigned _halfmoves = 0;

        bool _white_turn = true;

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

        Bitboard _white_castling = 0;
        Bitboard _black_castling = 0;

        // Defined in '/moves/<piece>_moves.cc
        void generate_pawn_moves(std::vector<Move> &moves);
        void generate_king_moves(std::vector<Move> &moves);
        void generate_knight_moves(std::vector<Move> &moves);
        void generate_sliding_moves(std::vector<Move> &moves, PieceType type);

        void generate_pawn_push_moves(std::vector<Move> &moves, Bitboard single_moves_bitboard,
                                      Bitboard double_moves_bitboard, Bitboard *piece_board);
        void generate_pawn_attack_moves(std::vector<Move> &moves, Bitboard moves_bitboard, Bitboard *piece_board,
                                        bool east_attacks);

        Bitboard get_attack_board();
    };
} // namespace board

#include "chessboard.hxx"
