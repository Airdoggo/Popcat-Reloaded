#include "chessboard.hh"

namespace board
{
    static const Bitboard RANK1 = 0x00000000000000FF;
    static const Bitboard RANK4 = 0x00000000FF000000;
    static const Bitboard RANK5 = 0x000000FF00000000;
    static const Bitboard RANK8 = 0xFF00000000000000;
    static const Bitboard NOTFILEA = 0xFEFEFEFEFEFEFEFE;
    static const Bitboard NOTFILEH = 0x7F7F7F7F7F7F7F7F;

    static void generate_single_push_moves(std::vector<Move> &moves, Bitboard moves_bitboard, Bitboard *piece_board,
                                           bool white_turn)
    {
        Bitboard promotion_rank = white_turn ? RANK8 : RANK1;

        while (moves_bitboard)
        {
            unsigned long long index = __lzcnt64(moves_bitboard);
            Bitboard move = 1ULL << (63 - index);
            moves_bitboard ^= move;
            move |= white_turn ? (move >> 8) : (move << 8);

            if (move & promotion_rank)
            {
                moves.push_back({ move, piece_board, nullptr, white_turn, MoveType::PROMOTION_QUEEN });
                moves.push_back({ move, piece_board, nullptr, white_turn, MoveType::PROMOTION_KNIGHT });
                moves.push_back({ move, piece_board, nullptr, white_turn, MoveType::PROMOTION_ROOK });
                moves.push_back({ move, piece_board, nullptr, white_turn, MoveType::PROMOTION_BISHOP });
            }
            else
            {
                moves.push_back({ move, piece_board, nullptr, white_turn, MoveType::NONE });
            }
        }
    }

    static void generate_double_push_moves(std::vector<Move> &moves, Bitboard moves_bitboard, Bitboard *piece_board,
                                           Bitboard enemy_pawns, bool white_turn)
    {
        Bitboard possible_en_passant_pawns = enemy_pawns & (white_turn ? RANK4 : RANK5);

        while (moves_bitboard)
        {
            unsigned long long index = __lzcnt64(moves_bitboard);
            Bitboard move = 1ULL << (63 - index);
            moves_bitboard ^= move;
            move |= white_turn ? (move >> 16) : (move << 16);

            MoveType type = (possible_en_passant_pawns && (((move >> 1) | (move << 1)) & possible_en_passant_pawns))
                ? MoveType::PASSING
                : MoveType::NONE;

            moves.push_back({ move, piece_board, nullptr, white_turn, type });
        }
    }

    static void generate_attacks(Chessboard *board, std::vector<Move> &moves, Bitboard moves_bitboard,
                                 Bitboard *piece_board, bool white_turn, bool east_attacks)
    {
        unsigned offset = (white_turn ^ east_attacks) ? 7 : 9;

        while (moves_bitboard)
        {
            unsigned long long index = __lzcnt64(moves_bitboard);
            Bitboard move = 1ULL << (63 - index);
            moves_bitboard ^= move;
            move |= white_turn ? (move >> offset) : (move << offset);

            moves.push_back(
                { move, piece_board, board->get_board_at_position(move, !white_turn), white_turn, MoveType::NONE });
        }
    }

    void Chessboard::generate_pawn_moves(std::vector<Move> &moves)
    {
        if (_white_turn)
        {
            Bitboard empty = ~(_whites | _blacks);

            Bitboard single_push = (_white_pawns << 8) & empty;

            Bitboard double_push = (single_push << 8) & empty & RANK4;

            generate_single_push_moves(moves, single_push, &_white_pawns, _white_turn);
            generate_double_push_moves(moves, double_push, &_white_pawns, _black_pawns, _white_turn);

            Bitboard enemy_pieces = _blacks | _en_passant;

            Bitboard east_attacks = (_white_pawns << 9) & NOTFILEA & enemy_pieces;
            Bitboard west_attacks = (_white_pawns << 7) & NOTFILEH & enemy_pieces;

            generate_attacks(this, moves, east_attacks, &_white_pawns, _white_turn, true);
            generate_attacks(this, moves, west_attacks, &_white_pawns, _white_turn, false);
        }
        else
        {
            Bitboard empty = ~(_whites | _blacks);

            Bitboard single_push = (_black_pawns >> 8) & empty;

            Bitboard double_push = (single_push >> 8) & empty & RANK5;

            generate_single_push_moves(moves, single_push, &_black_pawns, _white_turn);
            generate_double_push_moves(moves, double_push, &_black_pawns, _white_pawns, _white_turn);

            Bitboard enemy_pieces = _whites | _en_passant;

            Bitboard east_attacks = (_black_pawns >> 7) & NOTFILEA & enemy_pieces;
            Bitboard west_attacks = (_black_pawns >> 9) & NOTFILEH & enemy_pieces;

            generate_attacks(this, moves, east_attacks, &_black_pawns, _white_turn, true);
            generate_attacks(this, moves, west_attacks, &_black_pawns, _white_turn, false);
        }
    }
} // namespace board
