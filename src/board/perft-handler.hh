#pragma once

#include <string>

#include "chessboard.hh"

namespace board
{
    class PerftHandler final
    {
    public:
        void compute_perft(const std::string &perft_path, bool debug = false);

    private:
        size_t run_perft(Chessboard &board, unsigned depth);
        size_t run_verbose_perft(Chessboard &board, unsigned depth, std::string moves_history);
    };
} // namespace board