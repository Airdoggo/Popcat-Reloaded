add_library(ai
    "uci-handler.cc"
)
target_include_directories(ai PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" "{CMAKE_SOURCE_DIR}/src/board")

target_link_libraries(ai board)
