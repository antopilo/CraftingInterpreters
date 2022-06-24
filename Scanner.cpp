#include "Scanner.h"
#include "Lox.h"

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

bool Scanner::Match(char expected)
{
	if (IsAtEnd())
	{
		return false;
	}

	if (_source.at(_current) != expected)
	{
		return false;
	}

	_current++;
	return true;
}

char Scanner::Peek() const
{
	if (IsAtEnd())
	{
		return '\0';
	}

	return _source.at(_current);
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
		case '!': AddToken(Match('=') ? TokenType::BANG_EQUAL : TokenType::EQUAL); break;
		case '=': AddToken(Match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
		case '>': AddToken(Match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
		case '<': AddToken(Match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;
		case '/':
		{
			if (Match('/'))
			{
				// A comment goes until the end of the line.
				while (Peek() != '\n' && !IsAtEnd())
				{
					Advance();
				}
			}
			else
			{
				AddToken(TokenType::SLASH);
			}
			break;
		}
		case ' ':
		case '\r':
		case '\t':
			break;
		case '\n':
			_line++;
			break;

		default: Lox::Error(_line, "Unexpected character."); break;
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