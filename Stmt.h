#pragma once
#include <any>
#include "Expr/Expr.h"

#include "StmtVisitor.h"

class Stmt;
typedef std::shared_ptr<Stmt> StmtPtr;
class Stmt
{
public:
	Stmt() = default;
	virtual ~Stmt() = default;

	virtual std::any Accept(StmtVisitor<std::any>& visitor) const = 0;
};

class ExpressionStmt : public Stmt
{
private:
	ExprPtr _expr;

public:
	ExpressionStmt(ExprPtr expr)
	{
		_expr = expr;
	}

	std::any Accept(StmtVisitor<std::any>& visitor) const override
	{
		return visitor.visitExpressionStmt(*this);
	}

	ExprPtr GetExpression() const
	{
		return _expr;
	}
};

class PrintStmt : public Stmt
{
private:
	ExprPtr _expr;

public:
	PrintStmt(ExprPtr expr) : _expr(expr) {}

	std::any Accept(StmtVisitor<std::any>& visitor) const override
	{
		return visitor.visitPrintStmt(*this);
	}

	ExprPtr GetExpression() const { return _expr; }
};