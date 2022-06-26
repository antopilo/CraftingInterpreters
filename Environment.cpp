#include "Environment.h"
#include "RuntimeError.h"

Environment::Environment() : _values(std::map<std::string, std::any>()) {}

void Environment::Assign(Token name, std::any value)
{
	if (_values.find(name.GetLexeme()) != _values.end())
	{
		_values[name.GetLexeme()] = value;
		return;
	}

	throw RuntimeError(name, "Undefined variable '" + name.GetLexeme() + "'.");
}

void Environment::Define(const std::string& name, std::any value)
{
	_values[name] = value;
}

std::any Environment::Get(Token token)
{
	std::string lexeme = token.GetLexeme();
	if (_values.find(lexeme) != _values.end())
	{
		return _values[lexeme];
	}

	throw RuntimeError(token, "Undefined variable '" + lexeme + "'.");
}