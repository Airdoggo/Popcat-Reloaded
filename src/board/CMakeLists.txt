add_library(board
    "chessboard.cc"
    "perft-handler.cc"

    "moves/king-moves.cc"
    "moves/knight-moves.cc"
    "moves/pawn-moves.cc"
    "moves/sliding-moves.cc"
    "moves/validation.cc"
    "moves/tables.cc"
)

target_include_directories(board PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
