#pragma once

#include "chessboard.hh"

namespace ai
{
    std::string search(board::Chessboard *board, int depth);
    int evaluate(board::Chessboard *board);
} // namespace ai