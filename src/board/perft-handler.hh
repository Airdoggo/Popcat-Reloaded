#pragma once

#include <string>

#include "chessboard.hh"

namespace board
{
    class PerftHandler final
    {
    public:
        void compute_perft(const std::string &perft_path);

    private:
        Chessboard _board;

        size_t run_perft(unsigned depth);
    };
} // namespace board