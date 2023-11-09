#include "uci-handler.hh"

#include <iostream>
#include <sstream>

#include "ai.hh"

namespace ai
{
    static constexpr const char STARTPOS[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    static std::vector<std::string> get_command()
    {
        std::vector<std::string> command_parts;

        std::string input;
        std::getline(std::cin, input);
        std::stringstream in_stream(input);

        while (in_stream >> input)
        {
            command_parts.push_back(input);
        }

        return std::move(command_parts);
    }

    void UCIHandler::run()
    {
        std::vector<board::Move> moves;

        while (true)
        {
            std::vector<std::string> command = get_command();

            if (command.front() == "isready")
            {
                std::cout << "readyok" << std::endl;
            }

            else if (command.front() == "uci")
            {
                std::cout << "id name Popcat Reloaded" << std::endl
                          << "id author Airdoggo" << std::endl
                          << "uciok" << std::endl;
            }

            else if (command.front() == "ucinewgame")
            {
                handle_ucinewgame_command();
            }

            else if (command.front() == "position")
            {
                handle_position_command(command);
            }

            else if (command.front() == "go")
            {
                handle_go_command(command);
            }

            else if (command.front() == "quit")
                break;
        }
    }

    void UCIHandler::handle_position_command(const std::vector<std::string> &command)
    {
        bool from_startpos = command[1] == "startpos";

        if (_moves_offset == 0)
        {
            if (from_startpos)
                _ai.set_board_position(STARTPOS);
            else
            {
                std::string fen_string = command[2];
                for (int i = 3; i < 8; i++)
                    fen_string += " " + command[i];

                _ai.set_board_position(fen_string);
            }
        }

        for (size_t i = (from_startpos ? 3 : 9) + _moves_offset; i < command.size(); i++)
        {
            _ai.do_move(command[i]);
            _moves_offset++;
        }
    }

    void UCIHandler::handle_go_command(const std::vector<std::string> &command)
    {
        std::string result = _ai.search(5);

        std::cout << "bestmove " << result << std::endl;
    }

    void UCIHandler::handle_ucinewgame_command()
    {
        _moves_offset = 0;
    }
} // namespace ai