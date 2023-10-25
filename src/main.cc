#include <iostream>

#include "ai/tmp.hh"
#include "board/chessboard.hh"
#include "board/tmp.hh"
#include "options/tmp.hh"

int main(int argc, char *argv[])
{
    board::Chessboard board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    board.pretty_print();
    // options::tmp();
    // board::tmp();
    // ai::tmp();
    return 0;
}
