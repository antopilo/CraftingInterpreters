#pragma once
#include <any>
#include "Expr/Expr.h"
#include "Token.h"

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
		return visitor.VisitExpressionStmt(*this);
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
		return visitor.VisitPrintStmt(*this);
	}

	ExprPtr GetExpression() const { return _expr; }
};

class VarStmt : public Stmt
{
private:
	ExprPtr _initializer;
	Token _name;

public:
	VarStmt(Token op, ExprPtr initializer)
	{
		_initializer = initializer;
		_name = op;
	}

	std::any Accept(StmtVisitor<std::any>& visitor) const override
	{
		return visitor.VisitVarStmt(*this);
	}

	ExprPtr GetInitializer() const { return _initializer; }
	Token GetName() const { return _name; }
};