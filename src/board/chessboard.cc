#include "chessboard.hh"

#include <array>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

namespace board
{
    static constexpr Bitboard RANK3OR6 = 0x0000FF0000FF0000;
    static constexpr Bitboard RANK4OR5 = 0x000000FFFF000000;

    Chessboard::Chessboard()
        : Chessboard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
    {}

    Chessboard::Chessboard(const std::string &fen_string)
    {
        std::stringstream fen_stream(fen_string);
        std::string board, next_color, castling, en_passant;

        fen_stream >> board >> next_color >> castling >> en_passant >> _halfmoves >> _turn;

        std::unordered_map<char, Bitboard *> char_to_bitboard = {
            { 'r', &_black_rooks },   { 'n', &_black_knights }, { 'b', &_black_bishops }, { 'q', &_black_queen },
            { 'k', &_black_king },    { 'p', &_black_pawns },   { 'R', &_white_rooks },   { 'N', &_white_knights },
            { 'B', &_white_bishops }, { 'Q', &_white_queen },   { 'K', &_white_king },    { 'P', &_white_pawns }
        };

        int file = 1;
        int rank = 8;

        for (char c : board)
        {
            if (c == '/')
            {
                file = 1;
                rank--;
                continue;
            }

            if (c >= '1' && c <= '8')
                file += c - '0';
            else
                (*char_to_bitboard[c]) |= (1ULL << ((rank - 1) * 8 + (file++ - 1)));
        }

        _whites = _white_pawns | _white_rooks | _white_knights | _white_bishops | _white_queen | _white_king;
        _blacks = _black_pawns | _black_rooks | _black_knights | _black_bishops | _black_queen | _black_king;

        _white_turn = next_color[0] == 'w';

        if (castling[0] != '-')
        {
            _white_king_castling = castling.find('K') != std::string::npos;
            _white_queen_castling = castling.find('Q') != std::string::npos;
            _black_king_castling = castling.find('k') != std::string::npos;
            _black_queen_castling = castling.find('q') != std::string::npos;
        }

        if (en_passant != "-")
            _en_passant = 1ULL << (en_passant[0] - 'a' + (en_passant[1] - '1') * 8);
    }

    void Chessboard::generate_legal_moves(std::vector<Move> &moves)
    {
        generate_pawn_moves(moves);
        generate_king_moves(moves);
        generate_sliding_moves(moves, PieceType::ROOK);
        generate_sliding_moves(moves, PieceType::BISHOP);
        generate_sliding_moves(moves, PieceType::QUEEN);
        generate_knight_moves(moves);
    }

    void Chessboard::do_move(const Move &move)
    {
        if (move.type <= MoveType::PROMOTION_KNIGHT)
        {
            // Absolutely horrible, to change ASAP
            Bitboard *promotion_board = move.type == MoveType::PROMOTION_KNIGHT
                ? (move.is_white ? &_white_knights : &_black_knights)
                : move.type == MoveType::PROMOTION_BISHOP ? (move.is_white ? &_white_bishops : &_black_bishops)
                : move.type == MoveType::PROMOTION_ROOK   ? (move.is_white ? &_white_rooks : &_black_rooks)
                                                          : (move.is_white ? &_white_queen : &_black_queen);

            Bitboard move_start = ((*move.piece_board) | ~(*promotion_board)) & move.bitboard_move;
            Bitboard move_end = move.bitboard_move ^ move_start;

            (*move.piece_board) ^= move_start;
            (*promotion_board) ^= move_end;
        }
        else if (move.type <= MoveType::CASTLING_KING)
            return; // handle_castling(move);
        else
        {
            Bitboard *friend_pieces = _white_turn ? &_whites : &_blacks;
            Bitboard *enemy_pieces = _white_turn ? &_blacks : &_whites;

            *move.piece_board ^= move.bitboard_move;
            *friend_pieces ^= move.bitboard_move;

            if (move.target_board != nullptr)
            {
                // check_enemy_castling(move);

                if (move.type == MoveType::EN_PASSANT)
                {
                    Bitboard enemy = move.bitboard_move & RANK3OR6;
                    _en_passant ^= enemy;
                    enemy = (enemy << 8 | enemy >> 8) & RANK4OR5;

                    *move.target_board ^= enemy;
                    *enemy_pieces ^= enemy;
                }
                else
                {
                    Bitboard enemy =
                        (((*move.piece_board) & (*move.target_board)) ? (*move.piece_board) : ~(*move.piece_board))
                        & move.bitboard_move;

                    *move.target_board ^= enemy;
                    *enemy_pieces ^= enemy;
                }
            }
        }

        if (move.type != EN_PASSANT)
        {
            if (move.type == PASSING && !((*move.piece_board) & move.bitboard_move & RANK4OR5))
                _en_passant = (move.bitboard_move << 8 & move.bitboard_move >> 8);
            else
                _en_passant = 0x0;
        }
    }

    Bitboard *Chessboard::get_board_at_position(Bitboard position, bool is_white)
    {
        if (is_white)
        {
            if (position & _white_pawns)
                return &_white_pawns;
            else if (position & _white_rooks)
                return &_white_rooks;
            else if (position & _white_knights)
                return &_white_knights;
            else if (position & _white_bishops)
                return &_white_bishops;
            else if (position & _white_queen)
                return &_white_queen;
            else
                return &_white_king;
        }
        else
        {
            if (position & _black_pawns)
                return &_black_pawns;
            else if (position & _black_rooks)
                return &_black_rooks;
            else if (position & _black_knights)
                return &_black_knights;
            else if (position & _black_bishops)
                return &_black_bishops;
            else if (position & _black_queen)
                return &_black_queen;
            else
                return &_black_queen;
        }
    }

    void Chessboard::pretty_print() const
    {
        int board[64] = { 0 };
        char icons[13] = { '.', 'P', 'K', 'Q', 'R', 'B', 'N', 'p', 'k', 'q', 'r', 'b', 'n' };
        Bitboard bitboards[12] = { _white_pawns,   _white_king,    _white_queen,   _white_rooks,
                                   _white_bishops, _white_knights, _black_pawns,   _black_king,
                                   _black_queen,   _black_rooks,   _black_bishops, _black_knights };
        unsigned long index;

        for (unsigned i = 0; i < 12; i++)
        {
            Bitboard p_board = bitboards[i];

            while (_BitScanForward64(&index, p_board))
            {
                board[(7 - index / 8) * 8 + index % 8] = i + 1;
                p_board ^= (1ULL << index);
            }
        }

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                std::cout << icons[board[i * 8 + j]] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    void Chessboard::print_bitboard(Bitboard bitboard) const
    {
        for (unsigned i = 0; i < 8; i++)
        {
            for (unsigned j = 0; j < 8; j++)
            {
                std::cout << (bitboard & (1ULL << ((7 - i) * 8 + j)) ? 1 : 0) << " ";
            }

            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
} // namespace board
