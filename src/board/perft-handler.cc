#include "perft-handler.hh"

#include <fstream>
#include <iostream>
#include <vector>

#include "move.hh"

namespace board
{
    void PerftHandler::compute_perft(const std::string &perft_path)
    {
        std::ifstream file(perft_path);
        std::string perft_string;
        std::getline(file, perft_string);

        // To change if perft over a depth of 9 are added
        unsigned depth = perft_string.back() - '0';
        perft_string.pop_back();

        _board = Chessboard(perft_string);
        std::cout << run_perft(depth) << std::endl;
    }

    size_t PerftHandler::run_perft(unsigned depth)
    {
        std::vector<Move> moves;
        _board.generate_legal_moves(moves);
        if (depth <= 1)
            return moves.size();

        size_t nb_moves = 0;

        for (const Move &move : moves)
        {
            _board.do_move(move);
            _board.switch_turn();
            nb_moves += run_perft(depth - 1);
            _board.switch_turn();
            _board.do_move(move);
        }

        return nb_moves;
    }
} // namespace board
