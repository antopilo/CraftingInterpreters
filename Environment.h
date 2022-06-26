#pragma once

#include "Token.h"
#include "Memory.h"

#include <any>
#include <map>
#include <string>

class Environment
{
private:
	Environment* _enclosing;
	std::map<std::string, std::any> _values;

public:
	Environment();
	Environment(Ref<Environment> enclosing);

	~Environment();

	void Assign(Token name, std::any value);
	void Define(const std::string& name, std::any value);
	std::any Get(Token name);
};