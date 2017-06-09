cmake_minimum_required(VERSION 3.7)
project(combining_tree)

set(CMAKE_CXX_STANDARD 14)

find_package(Threads REQUIRED)
set(SOURCE_FILES main.cpp Node.cpp Node.h CStatus.h PanicException.h CombiningTree.cpp CombiningTree.h Timer.h)
add_executable(combining_tree ${SOURCE_FILES})

target_link_libraries(combining_tree Threads::Threads)