#include "move.hh"

namespace board
{
    std::string Move::to_string() const
    {
        std::string result = "";
        Bitboard move_start = bitboard_move & (*piece_board);
        Bitboard move_end = bitboard_move ^ move_start;

        unsigned long index;

        _BitScanForward64(&index, move_start);
        result += 'a' + (char)(index % 8);
        result += '1' + (char)(index / 8);

        _BitScanForward64(&index, move_end);
        result += 'a' + (char)(index % 8);
        result += '1' + (char)(index / 8);

        if (type & PROMOTION)
        {
            if (type & PROMOTION_BISHOP)
                result += 'b';
            else if (type & PROMOTION_ROOK)
                result += 'r';
            else if (type & PROMOTION_KNIGHT)
                result += 'n';
            else if (type & PROMOTION_QUEEN)
                result += 'q';
        }

        return result;
    }
} // namespace board
