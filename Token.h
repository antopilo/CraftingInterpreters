#pragma once
#include "TokenType.h"

#include <string>
#include <iostream>
#include <any>

class Token
{
private:
	TokenType _type;
	std::string _lexeme;
	std::any _literal;
	uint32_t _line;
public:
	Token(TokenType type, const std::string& lexeme, uint32_t line);
	Token(TokenType type, const std::string& lexeme, std::any literal, uint32_t line);
	Token() = default;
	~Token() = default;

	std::string ToString() const;
	std::string LiteralToString() const;
};
