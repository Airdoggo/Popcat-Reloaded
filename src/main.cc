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
        perft_handler.compute_perft("../../tests/perft/castling_rook/castling_rook_depth-3.perft", false);
    }
    else if (std::string(argv[1]) == "--perft")
    {
        board::PerftHandler perft_handler;
        perft_handler.compute_perft(argv[2]);
    }

    // options::tmp();
    // board::tmp();
    // ai::tmp();
    return 0;
}
