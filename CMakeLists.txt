cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)

set(COMPILER_FLAGS "-Wall -Wextra -Wpedantic -O3")
set(CMAKE_CXX_FLAGS "${COMPILER_FLAGS}")

project(Signals)

add_executable(Signals example.cpp SignalGuard.cpp)

