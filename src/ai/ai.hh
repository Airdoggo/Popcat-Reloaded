#pragma once

#include <unordered_map>

#include "chessboard.hh"
#include "piece-square-tables.hh"

#define COUNT 1

namespace ai
{
    class AI final
    {
    public:
        AI();

        void set_board_position(const std::string &fen_string);
        void do_move(const std::string &code);

        std::string search(int depth);

    private:
        int evaluate_move(int depth, int alpha, int beta);

        void evaluate_board();
        void update_evaluation(const board::Move &move, bool undo);

        board::Chessboard _board;

        int _early_evaluation;
        int _late_evaluation;
        int _phase;
        int _tapered_evaluation;

        const std::unordered_map<Bitboard *, tables::PieceSquareTable> _board_to_piece_square_table;
        const std::vector<tables::PieceSquareTable> _promotion_to_piece_square_table;
    };
} // namespace ai