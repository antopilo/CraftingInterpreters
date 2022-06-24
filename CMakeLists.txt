cmake_minimum_required(VERSION 3.10)

project(Lox)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES main.cpp Scanner.cpp)
set(HEADERS TokenType.h Scanner.h)

add_executable(Lox ${SOURCES} ${HEADERS})