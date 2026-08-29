cmake_minimum_required(VERSION 3.10)
project(a)

set(CMAKE_CXX_STANDARD 17)

include_directories(src external/chess-library/include)

add_executable(pgn_spy src/main.cpp src/analysis/analysis.cpp src/imports/imports.cpp src/imports/cc_imports.cpp src/imports/lc_imports.cpp src/reports/report.cpp)

target_link_libraries(pgn_spy pthread)
