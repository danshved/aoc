cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(aoc LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

# We just use the default build type. We want it fast (-O3), but we want
# to keep asserts (so no -DNDEBUG) and we want to fail on arithmetic overflow
# (-ftrapv). We also want a large stack for DFS.
add_compile_options(-ftrapv -O3)
add_link_options(-Wl,-z,stack-size=256000000)

include_directories(include)

file(GLOB SOLUTIONS "[0-9][0-9][0-9][0-9]/*/*.cpp")
foreach(PART_FILE ${SOLUTIONS})
    get_filename_component(PART ${PART_FILE} NAME_WE)

    get_filename_component(DAY_DIR ${PART_FILE} DIRECTORY)
    get_filename_component(DAY ${DAY_DIR} NAME_WE)

    get_filename_component(YEAR_DIR ${DAY_DIR} DIRECTORY)
    get_filename_component(YEAR ${YEAR_DIR} NAME_WE)

    add_executable(${YEAR}_${DAY}_${PART} ${PART_FILE})
endforeach()

add_executable(tests tests/tests.cpp)
