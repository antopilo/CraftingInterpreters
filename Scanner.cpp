#include "Scanner.h"

#include "Lox.h"

Scanner::Scanner(const std::string& source)
{
	_source = source;
	_tokens = std::vector<Token>();

	_keywords = std::map<std::string, TokenType>();
	_keywords["and"] = TokenType::AND;
	_keywords["class"] = TokenType::CLASS;
	_keywords["else"] = TokenType::ELSE;
	_keywords["false"] = TokenType::FALSE;
	_keywords["for"] = TokenType::FOR;
	_keywords["fun"] = TokenType::FUN;
	_keywords["if"] = TokenType::IF;
	_keywords["nil"] = TokenType::NIL;
	_keywords["or"] = TokenType::OR;
	_keywords["print"] = TokenType::PRINT;
	_keywords["return"] = TokenType::RETURN;
	_keywords["super"] = TokenType::SUPER;
	_keywords["this"] = TokenType::THIS;
	_keywords["true"] = TokenType::TRUE;
	_keywords["var"] = TokenType::VAR;
	_keywords["while"] = TokenType::WHILE;
}

std::vector<Token> Scanner::ScanTokens()
{
	while (!IsAtEnd())
	{
		_start = _current;
		ScanToken();
	}

	Token token = Token(TokenType::TokenEOF, "", _line);
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

char Scanner::PeekNext() const
{
	if (_current + 1 >= _source.size())
	{
		return '\0';
	}

	return _source.at(_current + 1);
}

bool Scanner::IsAlpha(char c) const
{
	return (c >= 'a' && c <= 'z') ||
		   (c >= 'A' && c <= 'Z') ||
		   c == '_';
}

bool Scanner::IsAlphaNumeric(char c) const
{
	return IsAlpha(c) || IsDigit(c);
}

bool Scanner::IsDigit(char c) const
{
	return c >= '0' && c <= '9';
}

void Scanner::String()
{
	while (Peek() != '"' && !IsAtEnd())
	{
		if (Peek() == '\n')
		{
			_line++;
		}
		Advance();
	}

	if (IsAtEnd())
	{
		Lox::Error(_line, "Unterminated string.");
		return;
	}

	// The closing ".
	Advance();

	// Trim the surrounding quotes.
	uint32_t trimStart = _start + 1;
	uint32_t trimEnd = _current - _start - 2;
	std::string value = _source.substr(trimStart, trimEnd);
	AddToken(TokenType::STRING, value);
}

void Scanner::Number()
{
	while (IsDigit(Peek()))
	{
		Advance();
	}

	if (Peek() == '.' && IsDigit(PeekNext()))
	{
		// Consume the "."
		Advance();

		while (IsDigit(Peek()))
		{
			Advance();
		}
	}

	std::string literalString = _source.substr(_start, _current - _start);
	double literal = atof(literalString.c_str());
	AddToken(TokenType::NUMBER, literal);
}

void Scanner::Identifier()
{
	while (IsAlphaNumeric(Peek()))
	{
		Advance();
	}

	TokenType type;
	std::string text = _source.substr(_start, _current - _start);
	if (_keywords.find(text) == _keywords.end())
	{
		type = TokenType::IDENTIFIER;
	}
	else
	{
		type = _keywords[text];
	}

	AddToken(type);
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
		case '>': AddToken(Match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;
		case '<': AddToken(Match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
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

		case '"': String(); break;
		default: 
		{
			if (IsDigit(c))
			{
				Number();
			}
			else if (IsAlpha(c))
			{
				Identifier();
			}
			else
			{
				Lox::Error(_line, "Unexpected character.");
			}
			break;
		}
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