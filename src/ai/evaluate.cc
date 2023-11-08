#include "ai.hh"

namespace ai
{
    static constexpr int QUEEN_VALUE = 1000;
    static constexpr int ROOK_VALUE = 525;
    static constexpr int BISHOP_VALUE = 350;
    static constexpr int KNIGHT_VALUE = 350;
    static constexpr int PAWN_VALUE = 100;

    inline static int get_material_value(const board::ColorBitboards *boards)
    {
        return (int)(__popcnt64(boards->queen) * QUEEN_VALUE + __popcnt64(boards->rooks) * ROOK_VALUE
                     + __popcnt64(boards->knights) * BISHOP_VALUE + __popcnt64(boards->bishops) * BISHOP_VALUE
                     + __popcnt64(boards->pawns) * PAWN_VALUE);
    }

    int evaluate(board::Chessboard *board)
    {
        board::ColorBitboards *whites = board->colors[0];
        board::ColorBitboards *blacks = board->colors[1];

        return (get_material_value(whites) - get_material_value(blacks)) * (board->white_turn ? 1 : -1);
    }
} // namespace ai