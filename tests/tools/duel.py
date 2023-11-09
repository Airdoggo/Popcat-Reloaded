import argparse
import subprocess
import chess

given_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1"

def get_engine_move(binary, position):

    stdin = position + '\n' + "go\n"

    res = subprocess.run([binary],
                          input=stdin,
                          stdout=subprocess.PIPE,
                          encoding="utf-8").stdout

    return res[res.find("bestmove") + 9:-1]

def play_game(ai1_binary, ai2_binary, ai1_turn):

    position = "position startpos moves"
    board = chess.Board()

    while (True):

        ai_to_move = ai1_binary if ai1_turn else ai2_binary
        move = get_engine_move(ai_to_move, position)

        board.push_uci(move)
        position += " " + move
        print(board, end="\n\n", flush=True)

        if board.is_checkmate():
            return 1 if ai1_turn else -1
        if board.is_fivefold_repetition() or board.is_fifty_moves():
            return 0

        ai1_turn ^= True

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Compare two AIs.")
    parser.add_argument("first_binary", type=str, help="The path to the first AI's binary.")
    parser.add_argument("second_binary", type=str, help="The path to the second AI's binary")
    parser.add_argument("-n", type=int, help="Number of duels, defaults to 1", default=1)
    args = parser.parse_args()

    n_wins_1 = 0
    n_wins_2 = 0

    for i in range(args.n):
        winner = play_game(args.first_binary, args.second_binary, i % 2 == 0)
        if winner == 1:
            n_wins_1 += 1
        elif winner == -1:
            n_wins_2 += 1

        print ("===========================")
        if winner != 0:
            print(f"AI {1 if winner == 1 else 2} won!")
        else:
            print("It's a draw!")
        print ("===========================")

    print ("===========================")
    print(f"First AI won {n_wins_1} times. Second AI won {n_wins_2} times.",
          "Number of draws: {args.n - n_wins_1 - n_wins_2}")
    print ("===========================")
