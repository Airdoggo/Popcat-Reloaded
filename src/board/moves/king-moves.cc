#include "chessboard.hh"
#include "tables.hh"

namespace board
{
    void Chessboard::generate_king_moves(std::vector<Move> &moves)
    {
        Bitboard *friendly_king = _white_turn ? &_white_king : &_black_king;
        Bitboard movable = _white_turn ? ~_whites : ~_blacks;
        Bitboard enemies = _white_turn ? _blacks : _whites;

        unsigned long long index = __lzcnt64(*friendly_king);
        Bitboard king_position = 1ULL << (63 - index);
        Bitboard moves_table = Tables::king[index] & movable;

        while (moves_table)
        {
            unsigned long long index = __lzcnt64(moves_table);
            Bitboard move = 1ULL << (63 - index);

            Bitboard *enemy = (move & enemies) ? get_board_at_position(move, !_white_turn) : nullptr;

            moves.push_back({ move | king_position, friendly_king, enemy, _white_turn, MoveType::NONE });

            moves_table ^= move;
        }
    }
} // namespace board
