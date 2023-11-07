#pragma once

#include "chessboard.hh"

namespace ai
{
    class UCIHandler final
    {
    public:
        void run();

    private:
        void handle_position_command(const std::vector<std::string> &command);
        void handle_go_command(const std::vector<std::string> &command);
        void handle_ucinewgame_command();

        board::Chessboard _board;

        size_t _moves_offset = 0;
    };
} // namespace ai