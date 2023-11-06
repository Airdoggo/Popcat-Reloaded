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

        ColorBitboards *whites = colors[0];
        ColorBitboards *blacks = colors[1];

        std::unordered_map<char, Bitboard *> char_to_bitboard = {
            { 'r', &blacks->_rooks }, { 'n', &blacks->_knights }, { 'b', &blacks->_bishops },
            { 'q', &blacks->_queen }, { 'k', &blacks->_king },    { 'p', &blacks->_pawns },
            { 'R', &whites->_rooks }, { 'N', &whites->_knights }, { 'B', &whites->_bishops },
            { 'Q', &whites->_queen }, { 'K', &whites->_king },    { 'P', &whites->_pawns }
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

        _whites =
            whites->_pawns | whites->_rooks | whites->_knights | whites->_bishops | whites->_queen | whites->_king;
        _blacks =
            blacks->_pawns | blacks->_rooks | blacks->_knights | blacks->_bishops | blacks->_queen | blacks->_king;

        whites->_friends = &_whites;
        whites->_enemies = &_blacks;
        blacks->_friends = &_blacks;
        blacks->_enemies = &_whites;

        _white_turn = next_color[0] == 'w';

        if (castling[0] != '-')
        {
            if (castling.find('K') != std::string::npos)
                whites->_castling |= WK_CASTLING;
            if (castling.find('Q') != std::string::npos)
                whites->_castling |= WQ_CASTLING;
            if (castling.find('k') != std::string::npos)
                blacks->_castling |= BK_CASTLING;
            if (castling.find('q') != std::string::npos)
                blacks->_castling |= BQ_CASTLING;
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
        ColorBitboards *color = move.is_white ? colors[0] : colors[1];

        if (move.type & MoveType::PROMOTION)
        {
            Bitboard *friend_pieces = color->_friends;
            Bitboard *enemy_pieces = color->_enemies;

            // Absolutely horrible, to change ASAP
            Bitboard *promotion_board = move.type & MoveType::PROMOTION_KNIGHT ? &color->_knights
                : move.type & MoveType::PROMOTION_BISHOP                       ? &color->_bishops
                : move.type & MoveType::PROMOTION_ROOK                         ? &color->_rooks
                                                                               : &color->_queen;

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
            Bitboard *friend_pieces = color->_friends;
            Bitboard *rook_board = &color->_rooks;

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
                Bitboard *castling = &color->_castling;

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
            Bitboard *friend_pieces = color->_friends;
            Bitboard *enemy_pieces = color->_enemies;

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
                Bitboard *castling = &color->_castling;

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
        ColorBitboards *color = is_white ? colors[0] : colors[1];

        if (position & color->_pawns)
            return &color->_pawns;
        else if (position & color->_rooks)
            return &color->_rooks;
        else if (position & color->_knights)
            return &color->_knights;
        else if (position & color->_bishops)
            return &color->_bishops;
        else if (position & color->_queen)
            return &color->_queen;
        else
            return &color->_king;
    }

    void Chessboard::pretty_print() const
    {
        int board[64] = { 0 };
        char icons[13] = { '.', 'P', 'K', 'Q', 'R', 'B', 'N', 'p', 'k', 'q', 'r', 'b', 'n' };

        ColorBitboards *whites = colors[0];
        ColorBitboards *blacks = colors[1];
        Bitboard bitboards[12] = { whites->_pawns,   whites->_king,    whites->_queen,   whites->_rooks,
                                   whites->_bishops, whites->_knights, blacks->_pawns,   blacks->_king,
                                   blacks->_queen,   blacks->_rooks,   blacks->_bishops, blacks->_knights };
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
