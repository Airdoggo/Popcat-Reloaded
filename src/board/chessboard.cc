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
        tables::initialize();
        set_board_from_fen(fen_string);
    }

    Chessboard::~Chessboard()
    {
        for (int i = 0; i < 64; i++)
        {
            delete[] tables::rook_attack_table[i];
            delete[] tables::bishop_attack_table[i];
        }
    }

    void Chessboard::set_board_from_fen(const std::string &fen_string)
    {
        reset_board();

        std::stringstream fen_stream(fen_string);
        std::string board, next_color, castling, en_passant;

        fen_stream >> board >> next_color >> castling >> en_passant >> _halfmoves >> _turn;

        std::unordered_map<char, Bitboard *> char_to_bitboard = {
            { 'r', &black_bitboards.rooks }, { 'n', &black_bitboards.knights }, { 'b', &black_bitboards.bishops },
            { 'q', &black_bitboards.queen }, { 'k', &black_bitboards.king },    { 'p', &black_bitboards.pawns },
            { 'R', &white_bitboards.rooks }, { 'N', &white_bitboards.knights }, { 'B', &white_bitboards.bishops },
            { 'Q', &white_bitboards.queen }, { 'K', &white_bitboards.king },    { 'P', &white_bitboards.pawns }
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

        _whites = white_bitboards.pawns | white_bitboards.rooks | white_bitboards.knights | white_bitboards.bishops
            | white_bitboards.queen | white_bitboards.king;
        _blacks = black_bitboards.pawns | black_bitboards.rooks | black_bitboards.knights | black_bitboards.bishops
            | black_bitboards.queen | black_bitboards.king;

        white_bitboards.friends = &_whites;
        white_bitboards.enemies = &_blacks;
        black_bitboards.friends = &_blacks;
        black_bitboards.enemies = &_whites;

        white_turn = next_color[0] == 'w';

        if (castling[0] != '-')
        {
            if (castling.find('K') != std::string::npos)
                white_bitboards.castling |= WK_CASTLING;
            if (castling.find('Q') != std::string::npos)
                white_bitboards.castling |= WQ_CASTLING;
            if (castling.find('k') != std::string::npos)
                black_bitboards.castling |= BK_CASTLING;
            if (castling.find('q') != std::string::npos)
                black_bitboards.castling |= BQ_CASTLING;
        }

        if (en_passant != "-")
            _en_passant = 1ULL << (en_passant[0] - 'a' + (en_passant[1] - '1') * 8);
    }

    void Chessboard::reset_board()
    {
        _whites = 0;
        _blacks = 0;

        white_bitboards.pawns = 0;
        white_bitboards.rooks = 0;
        white_bitboards.bishops = 0;
        white_bitboards.knights = 0;
        white_bitboards.queen = 0;
        white_bitboards.king = 0;
        white_bitboards.castling = 0;

        black_bitboards.pawns = 0;
        black_bitboards.rooks = 0;
        black_bitboards.bishops = 0;
        black_bitboards.knights = 0;
        black_bitboards.queen = 0;
        black_bitboards.king = 0;
        black_bitboards.castling = 0;
    }

    void Chessboard::generate_legal_moves(std::vector<Move> &moves)
    {
        generate_sliding_moves(moves, SlidingPieceType::QUEEN);
        generate_sliding_moves(moves, SlidingPieceType::ROOK);
        generate_sliding_moves(moves, SlidingPieceType::BISHOP);
        generate_knight_moves(moves);
        generate_pawn_moves(moves);
        generate_king_moves(moves);

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
        ColorBitboards *color = move.is_white ? &white_bitboards : &black_bitboards;

        if (move.type & MoveType::PROMOTION)
        {
            Bitboard *friend_pieces = color->friends;
            Bitboard *enemy_pieces = color->enemies;

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

    Bitboard *Chessboard::get_board_at_position(Bitboard position, bool is_white)
    {
        ColorBitboards *color = is_white ? &white_bitboards : &black_bitboards;

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

        Bitboard bitboards[12] = { white_bitboards.pawns, white_bitboards.king,    white_bitboards.queen,
                                   white_bitboards.rooks, white_bitboards.bishops, white_bitboards.knights,
                                   black_bitboards.pawns, black_bitboards.king,    black_bitboards.queen,
                                   black_bitboards.rooks, black_bitboards.bishops, black_bitboards.knights };
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
