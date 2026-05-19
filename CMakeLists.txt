cmake_minimum_required(VERSION 3.10)

project(BruteforceProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(benchmark CONFIG REQUIRED)

add_executable(bruteforce bruteforce.cpp optimal.cpp)

target_link_libraries(bruteforce PRIVATE benchmark::benchmark benchmark::benchmark_main)

file(COPY ${CMAKE_SOURCE_DIR}/testni_primeri
        DESTINATION ${CMAKE_BINARY_DIR})