#include "Lox.h"

namespace Lox
{
	void Report(uint32_t line, const std::string& where, const std::string& message)
	{
		std::cout << "[line " << line << "] Error" << where << ": " << message << std::endl;
		HadError = true;
	}

	void Error(uint32_t line, const std::string& message)
	{
		Report(line, "", message);
	}
}