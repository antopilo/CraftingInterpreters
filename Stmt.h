#pragma once
#include "Expr/Expr.h"
#include "Token.h"

#include "StmtVisitor.h"

#include <any>
#include <vector>

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

class BlockStmt : public Stmt
{
private:
	std::vector<StmtPtr> _statements;

public:
	BlockStmt(std::vector<StmtPtr> statements)
	{
		_statements = statements;
	}

	std::any Accept(StmtVisitor<std::any>& visitor) const override
	{
		return visitor.VisitBlockStmt(*this);
	}

	std::vector<StmtPtr> GetStatements() const { return _statements; }
};

class If : public Stmt
{
private:
	ExprPtr _condition;
	StmtPtr _thenBranch;
	StmtPtr _elseBranch;

public:
	If(ExprPtr condition, StmtPtr then, StmtPtr elseB) : _condition(condition), _thenBranch(then), _elseBranch(elseB)
	{

	}

	std::any Accept(StmtVisitor<std::any>& visitor) const override
	{
		return visitor.VisitIfStmt(*this);
	}

	auto GetCondition() const { return _condition; }
	auto GetThenBranch() const { return _thenBranch; }
	auto GetElseBranch() const { return _elseBranch; }
	bool HasElseBranch() const { return _elseBranch != nullptr; }
};

class While : public Stmt
{
private:
	ExprPtr _condition;
	StmtPtr _body;

public:
	While(ExprPtr cond, StmtPtr body) : _condition(cond), _body(body)
	{}

	std::any Accept(StmtVisitor<std::any>& visitor) const override
	{
		return visitor.VisitWhileStmt(*this);
	}

	auto GetCondition() const { return _condition; }
	auto GetBody() const { return _body; }
};

class Function : public Stmt
{
private:
	Token _name;
	std::vector<Token> _params;
	std::vector<StmtPtr> _body;

public:
	Function(Token name, std::vector<Token> params, std::vector<StmtPtr> body)
	{
		_name = name;
		_params = params;
		_body = body;
	}

	std::any Accept(StmtVisitor<std::any>& visitor) const override
	{
		return visitor.VisitFunctionStmt(*this);
	}

	auto GetName() const { return _name; }
	auto GetParams() const { return _params; }
	auto GetBody() const { return _body; }

};