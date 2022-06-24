#pragma once

#include "Token.h"

#include <map>
#include <vector>
#include <string>

class Scanner
{
private:
	std::string _source;
	std::vector<Token> _tokens;
	std::map<std::string, TokenType> _keywords;

	uint32_t _start = 0;
	uint32_t _current = 0;
	uint32_t _line = 1;

public:
	Scanner(const std::string& source);
	~Scanner() = default;

	std::vector<Token> ScanTokens();

private:
	bool IsAtEnd() const;
	bool Match(char expected);
	char Peek() const;
	char PeekNext() const;

	bool IsAlpha(char c) const;
	bool IsAlphaNumeric(char c) const;
	bool IsDigit(char c) const;

	void String();
	void Number();
	void Identifier();

	char Advance();
	void AddToken(TokenType type);
	void AddToken(TokenType type, std::any literal);
	void ScanToken();
};