#include <iostream>

#include "ai/tmp.hh"
#include "board/chessboard.hh"
#include "board/moves/tables.hh"
#include "board/perft-handler.hh"
#include "options/tmp.hh"

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        board::PerftHandler perft_handler;
        perft_handler.compute_perft("../../tests/perft/castling_base/castling_base_depth-5.perft", false);
    }
    else if (std::string(argv[1]) == "--perft")
    {
        board::PerftHandler perft_handler;
        perft_handler.compute_perft(argv[2]);
    }

    return 0;
}
