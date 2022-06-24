#pragma once
#include "Expr.h"
#include "../Token.h"
#include <vector>

class Binary : Expr
{
private:
	ExprPtr _Left;
	Token _Op;
	ExprPtr _Right;

public:
	Binary(ExprPtr left, Token op, ExprPtr right) 
	{
		_Left = std::move(left);
		_Op = op;
		_Right = std::move(right);
	}

	std::any Accept(ExprVisitor<std::any>& visitor) const override
	{
		return visitor.VisitBinaryExpr(*this);
	}

	const Expr& GetLeftExpr() const { return *_Left; }
	const Expr& GetRightExpr() const { return *_Right; }
	const Token& GetOp() const { return _Op; }
};

class Grouping : Expr
{
private:
	ExprPtr _Expression;

public:
	Grouping(ExprPtr exp)
	{
		_Expression = std::move(exp);
	}

	std::any Accept(ExprVisitor<std::any>& visitor) const override
	{
		return visitor.VisitGroupingExpr(*this);
	}
};

class Literal : Expr
{
private:
	std::any _Literal;

public:
	Literal(std::any lit)
	{
		_Literal = lit;
	}

	std::any Accept(ExprVisitor<std::any>& visitor) const override
	{
		return visitor.VisitLiteralExpr(*this);
	}
};

class Unary : Expr
{
private:
	Token _Op;
	ExprPtr _Right;

public:
	Unary(Token operation, ExprPtr right)
	{
		_Right = std::move(right);
		_Op = operation;
	}

	std::any Accept(ExprVisitor<std::any>& visitor) const override
	{
		return visitor.VisitUnaryExpr(*this);
	}
};

