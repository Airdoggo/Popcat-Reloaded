#include "ai.hh"

namespace ai
{
    AI::AI()
        : _board_to_piece_square_table({
            { &_board.white_bitboards.pawns, { tables::early_white_pawn, tables::end_white_pawn, 0 } },
            { &_board.black_bitboards.pawns, { tables::early_black_pawn, tables::end_black_pawn, 0 } },
            { &_board.white_bitboards.knights, { tables::early_white_knight, tables::end_white_knight, 1 } },
            { &_board.black_bitboards.knights, { tables::early_black_knight, tables::end_black_knight, 1 } },
            { &_board.white_bitboards.bishops, { tables::early_white_bishop, tables::end_white_bishop, 1 } },
            { &_board.black_bitboards.bishops, { tables::early_black_bishop, tables::end_black_bishop, 1 } },
            { &_board.white_bitboards.rooks, { tables::early_white_rook, tables::end_white_rook, 2 } },
            { &_board.black_bitboards.rooks, { tables::early_black_rook, tables::end_black_rook, 2 } },
            { &_board.white_bitboards.queen, { tables::early_white_queen, tables::end_white_queen, 4 } },
            { &_board.black_bitboards.queen, { tables::early_black_queen, tables::end_black_queen, 4 } },
            { &_board.white_bitboards.king, { tables::early_white_king, tables::end_white_king, 0 } },
            { &_board.black_bitboards.king, { tables::early_black_king, tables::end_black_king, 0 } },
        })
        , _promotion_to_piece_square_table({
              { tables::early_white_queen, tables::end_white_queen, 4 },
              { tables::early_white_rook, tables::end_white_rook, 2 },
              { tables::early_white_bishop, tables::end_white_bishop, 1 },
              { tables::early_white_knight, tables::end_white_knight, 1 },
              { tables::early_black_queen, tables::end_black_queen, 4 },
              { tables::early_black_rook, tables::end_black_rook, 2 },
              { tables::early_black_bishop, tables::end_black_bishop, 1 },
              { tables::early_black_knight, tables::end_black_knight, 1 },
          })
    {}

    void AI::set_board_position(const std::string &fen_string)
    {
        _board.set_board_from_fen(fen_string);
        evaluate_board();
    }

    void AI::do_move(const std::string &code)
    {
        std::vector<board::Move> moves;
        _board.generate_legal_moves(moves);

        for (const board::Move &move : moves)
        {
            if (move.to_string() == code)
            {
                update_evaluation(move, false);
                _board.do_move(move);
                break;
            }
        }

        _board.white_turn ^= true;
    }
} // namespace ai