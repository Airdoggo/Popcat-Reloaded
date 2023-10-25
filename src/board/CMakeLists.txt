add_library(board
    "tmp.cc"
    "chessboard.cc"
)
target_include_directories(board PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
