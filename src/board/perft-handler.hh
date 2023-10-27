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
        size_t run_perft(Chessboard &board, unsigned depth);
    };
} // namespace board