#include <iostream>

#include "ai/tmp.hh"
#include "board/tmp.hh"
#include "options/tmp.hh"

int main(int argc, char *argv[])
{
    std::cout << "Hello" << std::endl;
    options::tmp();
    board::tmp();
    ai::tmp();
    return 0;
}
