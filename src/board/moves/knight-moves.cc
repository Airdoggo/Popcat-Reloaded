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

        unsigned long index;

        while (_BitScanForward64(&index, knights))
        {
            Bitboard knight_position = 1ULL << index;
            Bitboard moves_table = _tables.knight[index] & movable;

            while (_BitScanForward64(&index, moves_table))
            {
                Bitboard move = 1ULL << index;
                Bitboard *enemy = (move & enemies) ? get_board_at_position(move, !_white_turn) : nullptr;

                moves.push_back({ move | knight_position, friendly_knights, enemy, _white_turn, MoveType::NONE });

                moves_table ^= move;
            }

            knights ^= knight_position;
        }
    }
} // namespace board
