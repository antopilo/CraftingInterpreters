#include "Scanner.h"

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

Scanner::Scanner(const std::string& source)
{
	_source = source;
	_tokens = std::vector<Token>();
}

std::vector<Token> Scanner::ScanTokens()
{
	while (!IsAtEnd())
	{
		_start = _current;
		ScanToken();
	}

	Token token = Token(TokenType::TokenEOF, std::string(""), _line);
	_tokens.push_back(token);
	return _tokens;
}

bool Scanner::IsAtEnd() const
{
	return _current >= _source.size();
}

void Scanner::ScanToken()
{
	char c = Advance();
	switch (c)
	{
		case '(': AddToken(TokenType::LEFT_PAREN); break;
		case ')': AddToken(TokenType::RIGHT_PAREN); break;
		case '{': AddToken(TokenType::LEFT_BRACE); break;
		case '}': AddToken(TokenType::RIGHT_BRACE); break;
		case ',': AddToken(TokenType::COMMA); break;
		case '.': AddToken(TokenType::DOT); break;
		case '-': AddToken(TokenType::MINUS); break;
		case '+': AddToken(TokenType::PLUS); break;
		case ';': AddToken(TokenType::SEMICOLON); break;
		case '*': AddToken(TokenType::STAR); break; 
	}
}

char Scanner::Advance()
{
	_current++;
	return _source.at(_current - 1);
}

void Scanner::AddToken(TokenType type)
{
	AddToken(type, nullptr);
}

void Scanner::AddToken(TokenType type, std::any literal)
{
	std::string text = _source.substr(_start, _current - _start);
	_tokens.push_back(Token(type, text, literal, _line));
}