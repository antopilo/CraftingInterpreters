#pragma once
#include "Token.h"
#include <exception>
#include <string>

class RuntimeError : public std::runtime_error
{
private:
	Token token;

public:
	RuntimeError(Token token, std::string message)
		: std::runtime_error(message), token(token)
	{ }

	RuntimeError() = default;

	Token GetToken() const { return token; }
};

class Return : public std::runtime_error
{
private:
	std::any _value;

public:
	Return(std::any value) : std::runtime_error(""), _value(value)
	{}

	auto GetValue() const { return _value; }
};