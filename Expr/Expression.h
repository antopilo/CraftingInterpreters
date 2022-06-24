#pragma once
#include "../Token.h"

class Expression
{

};

class Binary : Expression
{
public:
	Expression Left;
	Token Operator;
	Expression Right;

	Binary(Expression left, Token op, Expression right);
	~Binary() = default;
};