#include "Token.h"

Token::Token(TokenType type, const std::string& lexeme, uint32_t line)
{
	_type = type;
	_lexeme = lexeme;
	_literal = std::any{};
	_line = line;
}

Token::Token(TokenType type, const std::string& lexeme, std::any literal, uint32_t line)
{
	_type = type;
	_lexeme = lexeme;
	_literal = literal;
	_line = line;
}

std::string Token::ToString() const
{
	return std::to_string(static_cast<int>(_type)) + ", lexeme: '" + _lexeme + "' , literal: '" +
		LiteralToString() + "'";
}

std::string Token::LiteralToString() const
{
	switch (_type)
	{
	case TokenType::STRING:
		return std::any_cast<std::string>(_literal);
	case TokenType::NUMBER:
		return std::to_string(std::any_cast<double>(_literal));
	default:
		return "";
	}
}