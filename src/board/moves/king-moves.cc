#include "chessboard.hh"
#include "tables.hh"

namespace board
{
    void Chessboard::generate_king_moves(std::vector<Move> &moves)
    {
        Bitboard *friendly_king = _white_turn ? &_white_king : &_black_king;
        Bitboard movable = _white_turn ? ~_whites : ~_blacks;
        Bitboard enemies = _white_turn ? _blacks : _whites;

        unsigned long index;
        _BitScanForward64(&index, *friendly_king);

        Bitboard *friendly_pieces = _white_turn ? &_whites : &_blacks;
        *friendly_pieces ^= *friendly_king;
        Bitboard attack_board = get_attack_board();
        *friendly_pieces ^= *friendly_king;

        Bitboard moves_table = tables.king[index] & movable & ~attack_board;
        Bitboard castling = _white_turn ? _white_castling : _black_castling;

        MoveType type = MoveType::NONE;
        if (castling)
        {
            Bitboard castling_rooks =
                _white_turn ? _white_rooks & WR_STARTING_POSITION : _black_rooks & BR_STARTING_POSITION;

            if (castling & Q_CASTLING)
            {
                if (!(castling_rooks & RQ_CASTLING_START_POSITION)) // Enemy already broke queen castling
                {
                    _offset_castling_break =
                        static_cast<MoveType>(_offset_castling_break | MoveType::BREAK_CASTLING_QUEEN);
                }
                else
                    type = static_cast<MoveType>(type | MoveType::BREAK_CASTLING_QUEEN);
            }
            if (castling & K_CASTLING)
            {
                if (!(castling_rooks & RK_CASTLING_START_POSITION)) // Enemy already broke king castling
                {
                    _offset_castling_break =
                        static_cast<MoveType>(_offset_castling_break | MoveType::BREAK_CASTLING_KING);
                }
                else
                    type = static_cast<MoveType>(type | MoveType::BREAK_CASTLING_KING);
            }
        }

        while (_BitScanForward64(&index, moves_table))
        {
            Bitboard move = 1ULL << index;

            if (move & enemies)
            {
                Bitboard *enemy = get_board_at_position(move, !_white_turn);
                if (validate_move(friendly_king, enemy, *friendly_king, move))
                    moves.push_back({ move | *friendly_king, friendly_king, enemy, _en_passant, _white_turn, type });
            }
            else // Already validated
            {
                moves.push_back({ move | *friendly_king, friendly_king, nullptr, _en_passant, _white_turn, type });
            }

            moves_table ^= move;
        }

        if (get_attack_board() & (*friendly_king))
            return;

        if (type != MoveType::NONE)
        {
            Bitboard blockers = _whites | _blacks;

            Bitboard castling_queenside = _white_turn ? WQ_CASTLING : BQ_CASTLING;
            Bitboard castling_kingside = _white_turn ? WK_CASTLING : BK_CASTLING;

            if (!(castling_queenside & blockers) && type & MoveType::BREAK_CASTLING_QUEEN)
            {
                Bitboard castling_check = _white_turn ? WQ_CASTLING_CHECK : BQ_CASTLING_CHECK;

                if (!(castling_check & attack_board))
                {
                    moves.push_back({ (*friendly_king) >> 2 | *friendly_king, friendly_king, nullptr, _en_passant,
                                      _white_turn, static_cast<MoveType>(MoveType::CASTLING_QUEEN | type) });
                }
            }
            if (!(castling_kingside & blockers) && type & MoveType::BREAK_CASTLING_KING)
            {
                if (!(castling_kingside & attack_board))
                {
                    moves.push_back({ (*friendly_king) << 2 | *friendly_king, friendly_king, nullptr, _en_passant,
                                      _white_turn, static_cast<MoveType>(MoveType::CASTLING_KING | type) });
                }
            }
        }
    }
} // namespace board
