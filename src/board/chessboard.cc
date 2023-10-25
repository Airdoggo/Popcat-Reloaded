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

    Chessboard::Chessboard(std::string fen_string)
    {
        std::stringstream fen_stream(fen_string);
        std::string board, next_color, castling, en_passant;

        fen_stream >> board >> next_color >> castling >> en_passant >> _halfmoves >> _turn;

        std::unordered_map<char, Bitboard *> char_to_bitboard = {
            { 'r', &_black_rooks },   { 'n', &_black_knights }, { 'b', &_black_bishops }, { 'q', &_black_queen },
            { 'k', &_black_king },    { 'p', &_black_pawns },   { 'R', &_white_rooks },   { 'N', &_white_knights },
            { 'B', &_white_bishops }, { 'Q', &_white_queen },   { 'K', &_white_king },    { 'P', &_white_pawns }
        };

        int index = 63;

        for (char c : board)
        {
            if (c == '/')
                continue;

            if (c >= '1' && c <= '8')
                index -= c - '0';
            else
                (*char_to_bitboard[c]) |= (1ULL << index--);
        }

        _white_turn = next_color[0] == 'w';

        if (castling[0] != '-')
        {
            _white_king_castling = castling.find('K') != std::string::npos;
            _white_queen_castling = castling.find('Q') != std::string::npos;
            _black_king_castling = castling.find('k') != std::string::npos;
            _black_queen_castling = castling.find('q') != std::string::npos;
        }

        _en_passant = 1ULL << (en_passant[0] - 'a' + (en_passant[1] - '1') * 8);
    }

    void Chessboard::pretty_print() const
    {
        int board[64] = { 0 };
        char icons[13] = { '.', 'P', 'K', 'Q', 'R', 'B', 'N', 'p', 'k', 'q', 'r', 'b', 'n' };
        Bitboard bitboards[12] = { _white_pawns,   _white_king,    _white_queen,   _white_rooks,
                                   _white_bishops, _white_knights, _black_pawns,   _black_king,
                                   _black_queen,   _black_rooks,   _black_bishops, _black_knights };

        for (unsigned i = 0; i < 12; i++)
        {
            Bitboard p_board = bitboards[i];

            while (p_board)
            {
                unsigned long long index = __lzcnt64(p_board);
                board[index] = i + 1;
                p_board ^= (1ULL << (63 - index));
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

            std::cout << "\n";
        }
    }

} // namespace board
