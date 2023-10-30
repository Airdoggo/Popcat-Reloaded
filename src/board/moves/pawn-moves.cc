#include "chessboard.hh"

namespace board
{
    static constexpr Bitboard RANK1OR8 = 0xFF000000000000FF;
    static constexpr Bitboard RANK4 = 0x00000000FF000000;
    static constexpr Bitboard RANK5 = 0x000000FF00000000;
    static constexpr Bitboard NOTFILEA = 0xFEFEFEFEFEFEFEFE;
    static constexpr Bitboard NOTFILEH = 0x7F7F7F7F7F7F7F7F;

    void Chessboard::generate_pawn_push_moves(std::vector<Move> &moves, Bitboard single_moves_bitboard,
                                              Bitboard double_moves_bitboard, Bitboard *piece_board)
    {
        unsigned long index;

        while (_BitScanForward64(&index, single_moves_bitboard))
        {
            Bitboard move = 1ULL << index;
            Bitboard position = _white_turn ? (move >> 8) : (move << 8);

            single_moves_bitboard ^= move;

            if (validate_move(piece_board, nullptr, position, move))
            {
                move |= position;

                if (move & RANK1OR8)
                {
                    moves.push_back(
                        { move, piece_board, nullptr, _en_passant, _white_turn, MoveType::PROMOTION_QUEEN });
                    moves.push_back(
                        { move, piece_board, nullptr, _en_passant, _white_turn, MoveType::PROMOTION_KNIGHT });
                    moves.push_back({ move, piece_board, nullptr, _en_passant, _white_turn, MoveType::PROMOTION_ROOK });
                    moves.push_back(
                        { move, piece_board, nullptr, _en_passant, _white_turn, MoveType::PROMOTION_BISHOP });
                }
                else
                {
                    moves.push_back({ move, piece_board, nullptr, _en_passant, _white_turn, MoveType::NONE });
                }
            }
        }

        Bitboard possible_en_passant_pawns = _white_turn ? (_black_pawns & RANK4) : (_white_pawns & RANK5);

        while (_BitScanForward64(&index, double_moves_bitboard))
        {
            Bitboard move = 1ULL << index;
            Bitboard position = _white_turn ? (move >> 16) : (move << 16);

            MoveType type = (possible_en_passant_pawns && (((move >> 1) | (move << 1)) & possible_en_passant_pawns))
                ? MoveType::PASSING
                : MoveType::NONE;

            if (validate_move(piece_board, nullptr, position, move))
                moves.push_back({ position | move, piece_board, nullptr, _en_passant, _white_turn, type });

            double_moves_bitboard ^= move;
        }
    }

    void Chessboard::generate_pawn_attack_moves(std::vector<Move> &moves, Bitboard moves_bitboard,
                                                Bitboard *piece_board, bool east_attacks)
    {
        unsigned offset = (_white_turn ^ east_attacks) ? 7 : 9;
        unsigned long index;

        while (_BitScanForward64(&index, moves_bitboard))
        {
            Bitboard move = 1ULL << index;
            Bitboard position = _white_turn ? (move >> offset) : (move << offset);

            if (move & _en_passant)
            {
                Bitboard enemy_pawn_position = _white_turn ? move >> 8 : move << 8;
                Bitboard enemy_pawn_offset = move | enemy_pawn_position;
                Bitboard *enemy = get_board_at_position(enemy_pawn_position, !_white_turn);
                Bitboard *enemies = _white_turn ? &_blacks : &_whites;

                *enemy ^= enemy_pawn_offset;
                *enemies ^= enemy_pawn_offset;

                if (validate_move(piece_board, enemy, position, move))
                    moves.push_back(
                        { position | move, piece_board, enemy, _en_passant, _white_turn, MoveType::EN_PASSANT });

                *enemy ^= enemy_pawn_offset;
                *enemies ^= enemy_pawn_offset;
            }
            else
            {
                Bitboard *enemy = get_board_at_position(move, !_white_turn);

                if (validate_move(piece_board, enemy, position, move))
                {
                    if (move & RANK1OR8)
                    {
                        Bitboard full_move = position | move;
                        moves.push_back(
                            { full_move, piece_board, enemy, _en_passant, _white_turn, MoveType::PROMOTION_QUEEN });
                        moves.push_back(
                            { full_move, piece_board, enemy, _en_passant, _white_turn, MoveType::PROMOTION_KNIGHT });
                        moves.push_back(
                            { full_move, piece_board, enemy, _en_passant, _white_turn, MoveType::PROMOTION_ROOK });
                        moves.push_back(
                            { full_move, piece_board, enemy, _en_passant, _white_turn, MoveType::PROMOTION_BISHOP });
                    }
                    else
                    {
                        moves.push_back(
                            { position | move, piece_board, enemy, _en_passant, _white_turn, MoveType::NONE });
                    }
                }
            }

            moves_bitboard ^= move;
        }
    }

    void Chessboard::generate_pawn_moves(std::vector<Move> &moves)
    {
        Bitboard empty = ~(_whites | _blacks);

        if (_white_turn)
        {
            Bitboard single_push = (_white_pawns << 8) & empty;

            Bitboard double_push = (single_push << 8) & empty & RANK4;

            generate_pawn_push_moves(moves, single_push, double_push, &_white_pawns);

            Bitboard enemy_pieces = _blacks | _en_passant;

            Bitboard east_attacks = (_white_pawns << 9) & NOTFILEA & enemy_pieces;
            Bitboard west_attacks = (_white_pawns << 7) & NOTFILEH & enemy_pieces;

            generate_pawn_attack_moves(moves, east_attacks, &_white_pawns, true);
            generate_pawn_attack_moves(moves, west_attacks, &_white_pawns, false);
        }
        else
        {
            Bitboard single_push = (_black_pawns >> 8) & empty;

            Bitboard double_push = (single_push >> 8) & empty & RANK5;

            generate_pawn_push_moves(moves, single_push, double_push, &_black_pawns);

            Bitboard enemy_pieces = _whites | _en_passant;

            Bitboard east_attacks = (_black_pawns >> 7) & NOTFILEA & enemy_pieces;
            Bitboard west_attacks = (_black_pawns >> 9) & NOTFILEH & enemy_pieces;

            generate_pawn_attack_moves(moves, east_attacks, &_black_pawns, true);
            generate_pawn_attack_moves(moves, west_attacks, &_black_pawns, false);
        }
    }
} // namespace board
