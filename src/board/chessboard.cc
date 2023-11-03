#include "chessboard.hh"

#include <array>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

namespace board
{
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
            if (castling.find('K') != std::string::npos)
                _white_castling |= WK_CASTLING;
            if (castling.find('Q') != std::string::npos)
                _white_castling |= WQ_CASTLING;
            if (castling.find('k') != std::string::npos)
                _black_castling |= BK_CASTLING;
            if (castling.find('q') != std::string::npos)
                _black_castling |= BQ_CASTLING;
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

        // If the opponent broke a castling opportunity
        if (_offset_castling_break != MoveType::NONE)
        {
            for (Move &move : moves)
                move.type = static_cast<MoveType>(move.type | _offset_castling_break);

            _offset_castling_break = MoveType::NONE;
        }
    }

    void Chessboard::do_move(const Move &move)
    {
        if (move.type & MoveType::PROMOTION)
        {
            Bitboard *friend_pieces = _white_turn ? &_whites : &_blacks;
            Bitboard *enemy_pieces = _white_turn ? &_blacks : &_whites;

            // Absolutely horrible, to change ASAP
            Bitboard *promotion_board = move.type & MoveType::PROMOTION_KNIGHT
                ? (move.is_white ? &_white_knights : &_black_knights)
                : move.type & MoveType::PROMOTION_BISHOP ? (move.is_white ? &_white_bishops : &_black_bishops)
                : move.type & MoveType::PROMOTION_ROOK   ? (move.is_white ? &_white_rooks : &_black_rooks)
                                                         : (move.is_white ? &_white_queen : &_black_queen);

            Bitboard move_start = (*move.piece_board) & move.bitboard_move;
            Bitboard move_end = move_start ? move.bitboard_move ^ move_start : (*promotion_board) & move.bitboard_move;
            move_start = move_end ^ move.bitboard_move;

            (*move.piece_board) ^= move_start;
            (*promotion_board) ^= move_end;
            (*friend_pieces) ^= move.bitboard_move;

            if (move.target_board != nullptr)
            {
                *move.target_board ^= move_end;
                *enemy_pieces ^= move_end;
            }
        }
        else if (move.type & MoveType::CASTLING)
        {
            Bitboard *friend_pieces = _white_turn ? &_whites : &_blacks;
            Bitboard *rook_board = move.is_white ? &_white_rooks : &_black_rooks;

            *(move.piece_board) ^= move.bitboard_move;

            if (move.type & MoveType::CASTLING_KING)
            {
                Bitboard rook_move = move.is_white ? WK_CASTLING_ROOK_MOVE : BK_CASTLING_ROOK_MOVE;
                (*rook_board) ^= rook_move;
                (*friend_pieces) ^= move.bitboard_move | rook_move;
            }
            else
            {
                Bitboard rook_move = move.is_white ? WQ_CASTLING_ROOK_MOVE : BQ_CASTLING_ROOK_MOVE;
                (*rook_board) ^= rook_move;
                (*friend_pieces) ^= move.bitboard_move | rook_move;
            }

            if (move.type & MoveType::BREAK_BOTH_CASTLING)
            {
                Bitboard *castling = move.is_white ? &_white_castling : &_black_castling;

                if (move.type & MoveType::BREAK_CASTLING_KING)
                {
                    (*castling) ^= move.is_white ? WK_CASTLING : BK_CASTLING;
                }
                if (move.type & MoveType::BREAK_CASTLING_QUEEN)
                {
                    (*castling) ^= move.is_white ? WQ_CASTLING : BQ_CASTLING;
                }
            }
        }
        else
        {
            Bitboard *friend_pieces = _white_turn ? &_whites : &_blacks;
            Bitboard *enemy_pieces = _white_turn ? &_blacks : &_whites;

            *move.piece_board ^= move.bitboard_move;
            *friend_pieces ^= move.bitboard_move;

            if (move.target_board != nullptr)
            {
                if (move.type & MoveType::EN_PASSANT)
                {
                    Bitboard enemy = move.bitboard_move & RANK3OR6;
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

            if (move.type & MoveType::BREAK_BOTH_CASTLING)
            {
                Bitboard *castling = move.is_white ? &_white_castling : &_black_castling;

                if (move.type & MoveType::BREAK_CASTLING_KING)
                {
                    (*castling) ^= move.is_white ? WK_CASTLING : BK_CASTLING;
                }
                if (move.type & MoveType::BREAK_CASTLING_QUEEN)
                {
                    (*castling) ^= move.is_white ? WQ_CASTLING : BQ_CASTLING;
                }
            }
        }

        _en_passant ^= (move.type & MoveType::PASSING)
            ? move.en_passant | (move.bitboard_move << 8 & move.bitboard_move >> 8)
            : move.en_passant;
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
