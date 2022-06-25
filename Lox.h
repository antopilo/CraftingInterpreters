#pragma once
#include <string>
#include <iostream>

#include "RuntimeError.h"
#include "Token.h"

class Lox
{
public:
	static bool HadError;
	static bool HadRuntimeError;

	static void Report(uint32_t line, const std::string& where, const std::string& message);
	static void Error(uint32_t line, const std::string& message);
	static void Error(Token type, const std::string& message);
	static void RuntimeErr(RuntimeError error);
};