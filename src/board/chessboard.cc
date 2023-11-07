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
        set_board_from_fen(fen_string);
    }

    void Chessboard::set_board_from_fen(const std::string &fen_string)
    {
        reset_board();

        std::stringstream fen_stream(fen_string);
        std::string board, next_color, castling, en_passant;

        fen_stream >> board >> next_color >> castling >> en_passant >> _halfmoves >> _turn;

        ColorBitboards *whites = colors[0];
        ColorBitboards *blacks = colors[1];

        std::unordered_map<char, Bitboard *> char_to_bitboard = {
            { 'r', &blacks->rooks },   { 'n', &blacks->knights }, { 'b', &blacks->bishops }, { 'q', &blacks->queen },
            { 'k', &blacks->king },    { 'p', &blacks->pawns },   { 'R', &whites->rooks },   { 'N', &whites->knights },
            { 'B', &whites->bishops }, { 'Q', &whites->queen },   { 'K', &whites->king },    { 'P', &whites->pawns }
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

        _whites = whites->pawns | whites->rooks | whites->knights | whites->bishops | whites->queen | whites->king;
        _blacks = blacks->pawns | blacks->rooks | blacks->knights | blacks->bishops | blacks->queen | blacks->king;

        whites->friends = &_whites;
        whites->enemies = &_blacks;
        blacks->friends = &_blacks;
        blacks->enemies = &_whites;

        _white_turn = next_color[0] == 'w';

        if (castling[0] != '-')
        {
            if (castling.find('K') != std::string::npos)
                whites->castling |= WK_CASTLING;
            if (castling.find('Q') != std::string::npos)
                whites->castling |= WQ_CASTLING;
            if (castling.find('k') != std::string::npos)
                blacks->castling |= BK_CASTLING;
            if (castling.find('q') != std::string::npos)
                blacks->castling |= BQ_CASTLING;
        }

        if (en_passant != "-")
            _en_passant = 1ULL << (en_passant[0] - 'a' + (en_passant[1] - '1') * 8);
    }

    void Chessboard::reset_board()
    {
        _whites = 0;
        _blacks = 0;

        _white_bitboards.pawns = 0;
        _white_bitboards.rooks = 0;
        _white_bitboards.bishops = 0;
        _white_bitboards.knights = 0;
        _white_bitboards.queen = 0;
        _white_bitboards.king = 0;
        _white_bitboards.castling = 0;

        _black_bitboards.pawns = 0;
        _black_bitboards.rooks = 0;
        _black_bitboards.bishops = 0;
        _black_bitboards.knights = 0;
        _black_bitboards.queen = 0;
        _black_bitboards.king = 0;
        _black_bitboards.castling = 0;
    }

    void Chessboard::generate_legal_moves(std::vector<Move> &moves)
    {
        generate_pawn_moves(moves);
        generate_king_moves(moves);
        generate_sliding_moves(moves, SlidingPieceType::ROOK);
        generate_sliding_moves(moves, SlidingPieceType::BISHOP);
        generate_sliding_moves(moves, SlidingPieceType::QUEEN);
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
            Bitboard *friend_pieces = color->friends;
            Bitboard *enemy_pieces = color->enemies;

            // Absolutely horrible, to change ASAP
            Bitboard *promotion_board = move.type & MoveType::PROMOTION_KNIGHT ? &color->knights
                : move.type & MoveType::PROMOTION_BISHOP                       ? &color->bishops
                : move.type & MoveType::PROMOTION_ROOK                         ? &color->rooks
                                                                               : &color->queen;

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
            Bitboard *friend_pieces = color->friends;
            Bitboard *rook_board = &color->rooks;

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
                Bitboard *castling = &color->castling;

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
            Bitboard *friend_pieces = color->friends;
            Bitboard *enemy_pieces = color->enemies;

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
                Bitboard *castling = &color->castling;

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

    void Chessboard::do_move(const std::string &code)
    {
        std::vector<Move> moves;
        generate_legal_moves(moves);

        for (const Move &move : moves)
        {
            if (move.to_string() == code)
            {
                do_move(move);
                break;
            }
        }

        switch_turn();
    }

    Bitboard *Chessboard::get_board_at_position(Bitboard position, bool is_white)
    {
        ColorBitboards *color = is_white ? colors[0] : colors[1];

        if (position & color->pawns)
            return &color->pawns;
        else if (position & color->rooks)
            return &color->rooks;
        else if (position & color->knights)
            return &color->knights;
        else if (position & color->bishops)
            return &color->bishops;
        else if (position & color->queen)
            return &color->queen;
        else
            return &color->king;
    }

    void Chessboard::pretty_print() const
    {
        int board[64] = { 0 };
        char icons[13] = { '.', 'P', 'K', 'Q', 'R', 'B', 'N', 'p', 'k', 'q', 'r', 'b', 'n' };

        ColorBitboards *whites = colors[0];
        ColorBitboards *blacks = colors[1];
        Bitboard bitboards[12] = { whites->pawns,   whites->king,    whites->queen,   whites->rooks,
                                   whites->bishops, whites->knights, blacks->pawns,   blacks->king,
                                   blacks->queen,   blacks->rooks,   blacks->bishops, blacks->knights };
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
