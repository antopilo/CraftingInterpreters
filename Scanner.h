#pragma once
#include "TokenType.h"

#include <vector>
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
	~Token() = default;

	std::string ToString() const;

	std::string LiteralToString() const;
};


class Scanner
{
private:
	std::string _source;
	std::vector<Token> _tokens;

	uint32_t _start = 0;
	uint32_t _current = 0;
	uint32_t _line = 1;
public:
	Scanner(const std::string& source);
	~Scanner() = default;

	std::vector<Token> ScanTokens();

private:
	bool IsAtEnd() const;

	char Advance();
	void AddToken(TokenType type);
	void AddToken(TokenType type, std::any literal);
	void ScanToken();
};