#include "ai.hh"

namespace ai
{
    void AI::evaluate_board()
    {
        board::ColorBitboards *whites = &_board.white_bitboards;
        board::ColorBitboards *blacks = &_board.black_bitboards;

        Bitboard *all_pieces[12] = { &whites->pawns,   &blacks->pawns,   &whites->knights, &blacks->knights,
                                     &whites->bishops, &blacks->bishops, &whites->rooks,   &blacks->rooks,
                                     &whites->queen,   &blacks->queen,   &whites->king,    &blacks->king };

        unsigned long index;
        _early_evaluation = 0;
        _late_evaluation = 0;
        _phase = 0;

        for (int i = 0; i < 12; i++)
        {
            Bitboard current_pieces = *all_pieces[i];
            const tables::PieceSquareTable &piece_table = _board_to_piece_square_table.at(all_pieces[i]);

            while (_BitScanForward64(&index, current_pieces))
            {
                _phase += piece_table.phase_value;
                _early_evaluation += piece_table.early_table[index];
                _late_evaluation += piece_table.end_table[index];

                current_pieces ^= 1ULL << index;
            }
        }

        if (_phase > 24)
            _phase = 24;

        _tapered_evaluation = _early_evaluation * _phase + _late_evaluation * (24 - _phase);

#ifdef COUNT
        std::cout << "Initial evaluation: " << _tapered_evaluation << std::endl;
#endif
    }

    void AI::update_evaluation(const board::Move &move, bool undo)
    {
        unsigned long start_index;
        unsigned long end_index;

        Bitboard move_start = move.bitboard_move & (*move.piece_board);
        _BitScanForward64(&start_index, move_start);
        _BitScanForward64(&end_index, move.bitboard_move ^ move_start);

        const tables::PieceSquareTable &piece_table = _board_to_piece_square_table.at(move.piece_board);

        int early_delta = 0;
        int late_delta = 0;
        int phase_delta = 0;

        if (move.type & board::MoveType::PROMOTION)
        {
            unsigned long index;
            _BitScanForward64(&index, move.type);
            const tables::PieceSquareTable &promotion_table =
                _promotion_to_piece_square_table[_board.white_turn ? index : index + 4];

            early_delta += promotion_table.early_table[end_index] - piece_table.early_table[start_index];
            late_delta += promotion_table.end_table[end_index] - piece_table.end_table[start_index];
            phase_delta += promotion_table.phase_value;
        }
        else
        {
            early_delta += piece_table.early_table[end_index] - piece_table.early_table[start_index];
            late_delta += piece_table.end_table[end_index] - piece_table.end_table[start_index];
        }

        if (move.target_board)
        {
            const tables::PieceSquareTable &enemy_table = _board_to_piece_square_table.at(move.target_board);

            early_delta -= enemy_table.early_table[end_index];
            late_delta -= enemy_table.end_table[end_index];
            phase_delta -= enemy_table.phase_value;
        }

        if (undo)
        {
            _early_evaluation -= early_delta;
            _late_evaluation -= late_delta;
            _phase -= phase_delta;
        }
        else
        {
            _early_evaluation += early_delta;
            _late_evaluation += late_delta;
            _phase += phase_delta;
        }

        _tapered_evaluation = _early_evaluation * _phase + _late_evaluation * (24 - _phase);
    }
} // namespace ai