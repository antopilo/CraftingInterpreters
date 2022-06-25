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

	Token GetToken() const { return token; }
};