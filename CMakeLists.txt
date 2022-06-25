cmake_minimum_required(VERSION 3.10)

project(Lox)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES Lox.cpp main.cpp Scanner.cpp Token.cpp  )

set(HEADERS Lox.h TokenType.h Scanner.h Token.h Expr/Expr.h
Expr/ExprVisitor.h )

add_executable(Lox ${SOURCES} ${HEADERS})