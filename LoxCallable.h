#pragma once

#include "Callable.h"
#include "Environment.h"
#include "Interpreter.h"
#include "Memory.h"
#include "Stmt.h"

#include <any>
#include <vector>

class LoxFunction : public Callable
{
private:
	Function* _declaration = nullptr;
	Ref<Environment> _closure = nullptr;

public:
	LoxFunction(Function* declaration, Ref<Environment> closure)
	{
		_declaration = declaration;
		_closure = closure;
	}

	size_t GetArity() override
	{
		return _declaration->GetParams().size();
	}

	std::any Call(Interpreter& interpreter, std::vector<std::any> arguments) override
	{
		auto env = CreateRef<Environment>(_closure);
		auto params = _declaration->GetParams();
		for (uint32_t i = 0; i < std::size(params); i++)
		{
			auto& arg = params[i];
			auto lexem = arg.GetLexeme();
			env->Define(lexem, arguments[i]);
		}

		try 
		{
			interpreter.ExecuteBlock(_declaration->GetBody(), env);
		}
		catch (Return returnValue)
		{
			return returnValue.GetValue();
		}

		return nullptr;
	}

	std::string ToString() override
	{
		return "<fn " + _declaration->GetName().GetLexeme() + ">";
	}
};