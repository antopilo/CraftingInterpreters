#pragma once
#include <string>
#include <iostream>

namespace Lox
{
	static bool HadError = false;
	void Report(uint32_t line, const std::string& where, const std::string& message);
	void Error(uint32_t line, const std::string& message);
}