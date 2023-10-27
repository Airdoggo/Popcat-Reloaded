add_library(board
    "tmp.cc"
    "chessboard.cc"
    "perft-handler.cc"

    "moves/king-moves.cc"
    "moves/queen-moves.cc"
    "moves/bishop-moves.cc"
    "moves/knight-moves.cc"
    "moves/rook-moves.cc"
    "moves/pawn-moves.cc"
)
message("ASZASZASZASAZSAZSZAS" "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(board PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
