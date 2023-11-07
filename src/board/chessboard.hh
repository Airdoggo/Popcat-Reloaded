#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "constants.hh"
#include "moves/move.hh"
#include "moves/tables.hh"

typedef unsigned long long Bitboard;

namespace board
{
    struct ColorBitboards
    {
        Bitboard bishops = 0;
        Bitboard king = 0;
        Bitboard knights = 0;
        Bitboard pawns = 0;
        Bitboard queen = 0;
        Bitboard rooks = 0;

        Bitboard castling = 0;

        Bitboard *friends;
        Bitboard *enemies;
    };

    enum SlidingPieceType
    {
        ROOK = 0b1,
        BISHOP = 0b10,
        QUEEN = ROOK | BISHOP,
    };

    class Chessboard final
    {
    public:
        Tables tables;

        Chessboard();
        Chessboard(const std::string &fen_string);

        void set_board_from_fen(const std::string &fen_string);

        void pretty_print() const;
        void print_bitboard(Bitboard bitboard) const;

        void generate_legal_moves(std::vector<Move> &moves);
        void do_move(const Move &move);
        void do_move(const std::string &code);
        void switch_turn();

        Bitboard *get_board_at_position(Bitboard position, bool is_white);

        bool validate_move(Bitboard *moving_piece, Bitboard *target, Bitboard move_start, Bitboard move_end);

    private:
        unsigned _turn = 0;
        unsigned _halfmoves = 0;

        bool _white_turn = true;

        Bitboard _whites = 0;
        Bitboard _blacks = 0;

        ColorBitboards _white_bitboards;
        ColorBitboards _black_bitboards;

        ColorBitboards *colors[2] = { &_white_bitboards, &_black_bitboards };

        Bitboard _en_passant = 0;

        MoveType _offset_castling_break = MoveType::NONE;

        // Defined in '/moves/<piece>_moves.cc
        void generate_pawn_moves(std::vector<Move> &moves);
        void generate_king_moves(std::vector<Move> &moves);
        void generate_knight_moves(std::vector<Move> &moves);
        void generate_sliding_moves(std::vector<Move> &moves, SlidingPieceType type);

        void generate_pawn_push_moves(std::vector<Move> &moves, Bitboard single_moves_bitboard,
                                      Bitboard double_moves_bitboard, Bitboard *piece_board);
        void generate_pawn_attack_moves(std::vector<Move> &moves, Bitboard moves_bitboard, Bitboard *piece_board,
                                        bool east_attacks);

        Bitboard get_attack_board();
    };
} // namespace board

#include "chessboard.hxx"
