cmake_minimum_required(VERSION 3.16)

project(WordCounter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(COPY "${CMAKE_SOURCE_DIR}/file" DESTINATION "${CMAKE_BINARY_DIR}")

add_executable(WordCounter main.cpp)

target_compile_options(WordCounter PRIVATE -Wall -Wextra -Wpedantic -Wconversion)