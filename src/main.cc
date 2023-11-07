#include <iostream>

#include "ai/uci-handler.hh"
#include "board/chessboard.hh"
#include "board/moves/tables.hh"
#include "board/perft-handler.hh"
#include "options/tmp.hh"

int main(int argc, char *argv[])
{
    if (argc > 1 && std::string(argv[1]) == "--perft")
    {
        board::PerftHandler perft_handler;
        perft_handler.compute_perft(argv[2]);
    }
    else
    {
        ai::UCIHandler uci_handler;
        uci_handler.run();
    }

    return 0;
}
