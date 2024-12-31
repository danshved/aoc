cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(aoc LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

# We just use the default build type. We want it fast (-O3), but we want
# to keep asserts (so no -DNDEBUG) and we want to fail on arithmetic overload
# (-ftrapv). We also want a large stack for DFS.
add_compile_options(-ftrapv -O3)
add_link_options(-Wl,--stack,256000000)

include_directories(include)

add_executable(tests tests/tests.cpp)