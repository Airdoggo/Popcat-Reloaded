#include "chessboard.hh"
#include "tables.hh"

namespace board
{
    void Chessboard::generate_knight_moves(std::vector<Move> &moves)
    {
        ColorBitboards *color = _white_turn ? colors[0] : colors[1];
        Bitboard *friendly_knights = &color->knights;
        Bitboard knights = *friendly_knights;
        Bitboard movable = ~(*color->friends);
        Bitboard enemies = *color->enemies;

        unsigned long index;

        while (_BitScanForward64(&index, knights))
        {
            Bitboard knight_position = 1ULL << index;
            Bitboard moves_table = tables.knight[index] & movable;

            while (_BitScanForward64(&index, moves_table))
            {
                Bitboard move = 1ULL << index;
                Bitboard *enemy = (move & enemies) ? get_board_at_position(move, !_white_turn) : nullptr;

                if (validate_move(friendly_knights, enemy, knight_position, move))
                    moves.push_back(
                        { move | knight_position, friendly_knights, enemy, _en_passant, _white_turn, MoveType::NONE });

                moves_table ^= move;
            }

            knights ^= knight_position;
        }
    }
} // namespace board
