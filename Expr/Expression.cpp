#include "Expression.h"

Binary::Binary(Expression left, Token op, Expression right)
{
	Left = left;
	Operator = op;
	Right = right;
}