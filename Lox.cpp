#include "Lox.h"

bool Lox::HadError = false;

void Lox::Report(uint32_t line, const std::string& where, const std::string& message)
{
	std::cout << "[line " << line << "] Error" << where << ": " << message << std::endl;
	HadError = true;
}

void Lox::Error(uint32_t line, const std::string& message)
{
	Report(line, "", message);
}

void Lox::Error(Token token, const std::string& message)
{
	if (token.GetType() == TokenType::TokenEOF)
	{
		Report(token.GetLine(), " at end", message);
	}
	else
	{
		Report(token.GetLine(), " at '" + token.GetLexeme() + "'", message);
	}
}