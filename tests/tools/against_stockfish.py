import argparse
import os.path
import subprocess
import chess
from stockfish import Stockfish

given_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1"

def get_engine_move(binary, position):

    stdin = position + '\n' + "go\n"

    res = subprocess.run([binary],
                          input=stdin,
                          stdout=subprocess.PIPE,
                          encoding="utf-8").stdout

    return res[res.find("bestmove") + 9:-1]

def play_game(ai_binary, stockfish_binary, level, ai_turn):

    position = "position startpos moves"
    stockfish = Stockfish(stockfish_binary)
    stockfish.set_skill_level(level)
    board = chess.Board()

    while (True):

        move = get_engine_move(ai_binary, position) if ai_turn else stockfish.get_best_move()

        board.push_uci(move)
        stockfish.make_moves_from_current_position([move])
        position += " " + move
        print(board, end="\n\n", flush=True)

        if board.is_checkmate():
            return 1 if ai_turn else -1
        if board.is_fivefold_repetition() or board.is_fifty_moves():
            return 0

        ai_turn ^= True

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Compare an AI with Stockfish.")
    parser.add_argument("ai_binary", type=str, help="The path to the your AI's binary.")
    parser.add_argument("stockfish_binary", type=str, help="The path to Stockfish's binary")
    parser.add_argument("level", type=str, help="The level of Stockfish")
    parser.add_argument("-n", type=int, help="Number of duels, defaults to 1", default=1)
    args = parser.parse_args()

    n_wins_1 = 0
    n_wins_2 = 0

    for i in range(args.n):
        winner = play_game(args.ai_binary, args.stockfish_binary, args.level, i % 2 == 0)
        if winner == 1:
            n_wins_1 += 1
        elif winner == -1:
            n_wins_2 += 1

        print ("===========================")
        if winner == 1:
            print(f"Your AI won!")
        elif winner == -1:
            print("Stockfish wins!")
        else:
            print("It's a draw!")
        print ("===========================")

    print ("===========================")
    print(f"The AI won {n_wins_1} times. Stockfish won {n_wins_2} times.",
          f"Number of draws: {args.n - n_wins_1 - n_wins_2}")
    print ("===========================")
