#include "ai.hh"

#define COUNT 1

namespace ai
{
    static constexpr int MIN_SCORE = INT_MIN + 100;

#ifdef COUNT
    static int nb_inner = 0;
    static int nb_leaves = 0;
#endif

    static int evaluate_move(board::Chessboard *board, int depth, int alpha, int beta)
    {
        std::vector<board::Move> moves;
        board->generate_legal_moves(moves);

        if (moves.empty())
        {
#ifdef COUNT
            nb_inner++;
#endif
            if (board->is_in_check()) // Mat
            {
                return MIN_SCORE - depth;
            }

            return 0; // Pat
        }

        for (const board::Move &move : moves)
        {
            board->do_move(move);
            board->switch_turn();

            int eval;

            if (depth <= 1 && move.target_board == nullptr || depth <= -2)
            {
#ifdef COUNT
                nb_leaves++;
#endif
                eval = -evaluate(board);
            }
            else
            {
#ifdef COUNT
                nb_inner++;
#endif
                eval = -evaluate_move(board, depth - 1, -beta, -alpha);
            }

            board->switch_turn();
            board->do_move(move);

            if (eval >= beta)
            {
                return beta;
            }

            if (eval > alpha)
                alpha = eval;
        }

        return alpha;
    }

    std::string search(board::Chessboard *board, int depth)
    {
#ifdef COUNT
        nb_inner = 0;
        nb_leaves = 0;
#endif

        std::vector<board::Move> moves;
        board->generate_legal_moves(moves);

        board::Move &best_move = moves.front();
        int best_score = MIN_SCORE;

        for (const board::Move &move : moves)
        {
#ifdef COUNT
            nb_inner++;
#endif

            board->do_move(move);
            board->switch_turn();

            int eval = -evaluate_move(board, depth, MIN_SCORE, -best_score);

            if (eval > best_score)
            {
                best_score = eval;
                best_move = move;
            }

            board->switch_turn();
            board->do_move(move);
        }

#ifdef COUNT
        std::cout << "Total nodes searched: " << nb_inner + nb_leaves << " (inner: " << nb_inner
                  << ", leaves: " << nb_leaves << ") | score: " << best_score << std::endl;
#endif

        return best_move.to_string();
    }
} // namespace ai