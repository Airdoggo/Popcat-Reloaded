#include "chessboard.hh"
#include "tables.hh"

namespace board
{
    void Chessboard::generate_knight_moves(std::vector<Move> &moves)
    {
        Bitboard *friendly_knights = _white_turn ? &_white_knights : &_black_knights;
        Bitboard knights = *friendly_knights;
        Bitboard movable = _white_turn ? ~_whites : ~_blacks;
        Bitboard enemies = _white_turn ? _blacks : _whites;

        while (knights)
        {
            unsigned long long index = __lzcnt64(knights);
            Bitboard knight_position = 1ULL << (63 - index);
            Bitboard moves_table = Tables::knight[index] & movable;

            while (moves_table)
            {
                unsigned long long index = __lzcnt64(moves_table);
                Bitboard move = 1ULL << (63 - index);

                Bitboard *enemy = (move & enemies) ? get_board_at_position(move, !_white_turn) : nullptr;

                if (enemy != nullptr && false)
                {
                    std::cout << "lol" << std::endl;
                    print_bitboard(move);
                    print_bitboard(knight_position);
                    print_bitboard(knights);
                    exit(1);
                }

                moves.push_back({ move | knight_position, friendly_knights, enemy, _white_turn, MoveType::NONE });

                moves_table ^= move;
            }

            knights ^= knight_position;
        }
    }
} // namespace board
