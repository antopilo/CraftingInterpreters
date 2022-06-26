#pragma once

#include <any>
#include <vector>
#include <chrono>

class Interpreter;

class Callable
{
public:
	Callable() = default;
	virtual ~Callable() = default;

	virtual uint32_t GetArity() 
	{ 
		return 0; 
	}

	virtual std::any Call(Interpreter& interpreter, std::vector<std::any> arguments)
	{
		return nullptr;
	}
	virtual std::string ToString() 
	{ 
		return "<native fn>"; 
	};
};

class Clock : public Callable
{
public:

	uint32_t GetArity() override
	{
		return 0;
	}

	std::any Call(Interpreter& interpreter, std::vector<std::any> arguments) override
	{
		auto millisec_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		return (double)millisec_since_epoch;
	}

	std::string ToString() override
	{
		return "<native fn>";
	}
};
