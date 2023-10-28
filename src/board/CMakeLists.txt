add_library(board
    "chessboard.cc"
    "perft-handler.cc"

    "moves/king-moves.cc"
    "moves/knight-moves.cc"
    "moves/pawn-moves.cc"
    "moves/sliding-moves.cc"
    "moves/tables.cc"
)
message("ASZASZASZASAZSAZSZAS" "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(board PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
