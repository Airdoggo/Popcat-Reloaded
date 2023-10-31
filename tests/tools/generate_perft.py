import argparse
import argparse
from chess import Board


def perft(depth, board, moves = ""):
    if depth == 1:
        return len(list(board.legal_moves))

    res = 0
    for move in board.legal_moves:
        new_moves = moves + " " + str(move)

        board.push(move)
        perft_result = perft(depth - 1, board, new_moves)
        res += perft_result
        board.pop()

        print(new_moves, perft_result, flush=True)

    return res

def generate_perft(fen, depth):
    board = Board(fen)
    result = perft(depth, board)
    print("Result:", result)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate perft.")
    parser.add_argument("fen", type=str, help="The FEN string")
    parser.add_argument("depths", type=int, help="The max number of depths")
    args = parser.parse_args()

    generate_perft(args.fen, args.depths)
