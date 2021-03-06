#include "Environment.h"
#include "RuntimeError.h"

Environment::Environment() : _values(std::map<std::string, std::any>()) 
{
	_enclosing = nullptr;
}

Environment::Environment(Ref<Environment> enclosing)
	: _values(std::map<std::string, std::any>())
{
	_enclosing = enclosing;
}

Environment::~Environment()
{
}

void Environment::Assign(Token name, std::any value)
{
	if (_values.find(name.GetLexeme()) != _values.end())
	{
		_values[name.GetLexeme()] = value;
		return;
	}

	if (_enclosing != nullptr)
	{
		_enclosing->Assign(name, value);
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

	if (_enclosing != nullptr)
	{
		return _enclosing->Get(token);
	}

	throw RuntimeError(token, "Undefined variable '" + lexeme + "'.");
}

std::any Environment::GetAt(int distance, std::string name)
{
	return Ancestor(distance)->GetValues()[name];
}

Ref<Environment> Environment::Ancestor(int distance)
{
	std::shared_ptr<Environment> env = std::shared_ptr<Environment>(this);
	for (int i = 0; i < distance; i++)
	{
		env = env->_enclosing;
	}

	return env;
}
