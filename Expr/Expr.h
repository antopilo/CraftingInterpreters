#pragma once

#include "ExprVisitor.h"
#include "../Token.h"

#include <memory>
#include <any>



class Expr;
typedef std::shared_ptr<Expr> ExprPtr;
class Expr
{
public:
	virtual std::any Accept(ExprVisitor<std::any>& visitor) const = 0;
	Expr() = default;
	virtual ~Expr() = default;
};

class Binary : public Expr
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

	~Binary() override = default;

	std::any Accept(ExprVisitor<std::any>& visitor) const override
	{
		return visitor.VisitBinaryExpr(*this);
	}

	const ExprPtr GetLeftExpr() const { return _Left; }
	const ExprPtr GetRightExpr() const { return _Right; }
	const Token& GetOp() const { return _Op; }
};

class Grouping : public Expr
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

	const ExprPtr GetExpr() const { return _Expression; }
};

enum class LiteralType
{
	Double, String
};

class Literal : public Expr
{
private:
	std::any _Literal;
	LiteralType _Type;
public:
	Literal(std::any lit)
	{
		_Literal = lit;
	}

	std::any Accept(ExprVisitor<std::any>& visitor) const override
	{
		return visitor.VisitLiteralExpr(*this);
	}

	const std::any& GetLiteral() const { return _Literal; }
};

class Unary : public Expr
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

	~Unary() override = default;

	std::any Accept(ExprVisitor<std::any>& visitor) const override
	{
		return visitor.VisitUnaryExpr(*this);
	}


	const Token& GetOperator() const { return _Op; }
	const ExprPtr GetRight() const { return _Right; }
};


class Var : public Expr
{
private:
	Token _name;

public:
	Var(Token name)
	{
		_name = name;
	}

	std::any Accept(ExprVisitor<std::any>& visitor) const override
	{
		return visitor.VisitVariableExpr(*this);
	}

	Token GetName() const { return _name; }
};