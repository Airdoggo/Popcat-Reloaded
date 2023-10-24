add_library(ai
    "tmp.cc"
)
target_include_directories(ai PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(ai board)
