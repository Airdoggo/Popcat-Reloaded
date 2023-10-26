#include <iostream>

#include "ai/tmp.hh"
#include "board/chessboard.hh"
#include "board/perft-handler.hh"
#include "board/tmp.hh"
#include "options/tmp.hh"

int main(int argc, char *argv[])
{
    board::PerftHandler perft_handler;
    perft_handler.compute_perft("../../tests/perft/base/base_depth-1.perft");
    // options::tmp();
    // board::tmp();
    // ai::tmp();
    return 0;
}
