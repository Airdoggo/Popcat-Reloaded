#include "perft-handler.hh"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#include "moves/move.hh"

namespace board
{
    void PerftHandler::compute_perft(const std::string &perft_path, bool debug)
    {
        std::ifstream file(perft_path);
        if (!file.is_open())
        {
            std::cout << -1 << std::endl;
            return;
        }

        std::string perft_string;
        std::getline(file, perft_string);

        // To change if perft over a depth of 9 are added
        unsigned depth = perft_string.back() - '0';
        perft_string.pop_back();

        Chessboard board = Chessboard(perft_string);

        if (!debug)
            std::cout << run_perft(board, depth) << std::endl;
        else
            std::cout << run_verbose_perft(board, depth, "") << std::endl;
    }

    size_t PerftHandler::run_perft(Chessboard &board, unsigned depth)
    {
        std::vector<Move> moves;
        board.generate_legal_moves(moves);

        if (depth <= 1)
            return moves.size();

        size_t nb_moves = 0;

        for (const Move &move : moves)
        {
            board.do_move(move);
            board.switch_turn();

            nb_moves += run_perft(board, depth - 1);

            board.switch_turn();
            board.do_move(move);
        }

        return nb_moves;
    }

    size_t PerftHandler::run_verbose_perft(Chessboard &board, unsigned depth, std::string moves_history)
    {
        std::vector<Move> moves;
        board.generate_legal_moves(moves);

        if (depth <= 1)
            return moves.size();

        size_t nb_moves = 0;

        std::sort(moves.begin(), moves.end(), [](const Move &a, const Move &b) {
            return (a.bitboard_move & (*a.piece_board)) < (b.bitboard_move & (*b.piece_board));
        });

        for (const Move &move : moves)
        {
            std::string new_history = moves_history + (moves_history.empty() ? "" : " ") + move.to_string();

            board.do_move(move);
            board.switch_turn();

            size_t perft_result = run_verbose_perft(board, depth - 1, new_history);
            nb_moves += perft_result;

            std::cout << new_history << " " << perft_result << std::endl;

            board.switch_turn();
            board.do_move(move);
        }

        return nb_moves;
    }
} // namespace board
