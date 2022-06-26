#pragma once
#include "StmtVisitor.h"

#include "Interpreter.h"
#include "Lox.h"
#include "Memory.h"

#include <any>
#include <map>
#include <stack>
#include <vector>

class Resolver : public StmtVisitor<std::any>, public ExprVisitor<std::any>
{
private:
	Ref<Interpreter> _interpreter;
	std::vector<std::map<std::string, bool>> _scopes;

public:
	Resolver(Ref<Interpreter> interpreter)
		: _interpreter(interpreter)
	{ }

	std::any VisitBlockStmt(const BlockStmt& stmt) override
	{
		BeginScope();
		Resolve(stmt.GetStatements());
		EndScope();
		return nullptr;
	}

	std::any VisitExpressionStmt(const If& stmt)
	{
		Resolve(stmt.GetCondition());
		Resolve(stmt.GetThenBranch());
		if (stmt.GetElseBranch() != nullptr)
		{
			Resolve(stmt.GetElseBranch());
		}
		return nullptr;
	}

	std::any VisitFunctionStmt(const Function& stmt) override
	{
		Declare(stmt.GetName());
		Define(stmt.GetName());
		ResolveFunction(stmt);
		return nullptr;
	}

	std::any VisitPrintStmt(const PrintStmt& stmt) override
	{
		Resolve(stmt.GetExpression());
		return nullptr;
	}

	std::any VisitReturnStmt(const ReturnStmt& stmt) override
	{
		if (stmt.GetValue() != nullptr)
		{
			Resolve(stmt.GetValue());
		}

		return nullptr;
	}

	std::any VisitVarStmt(const VarStmt& stmt) override
	{
		Declare(stmt.GetName());
		if (stmt.GetInitializer() != nullptr)
		{
			Resolve(stmt.GetInitializer());
		}
		Define(stmt.GetName());
		return nullptr;
	}
	 
	std::any VisitAssignExpr(const Assign& expr) override
	{
		Resolve(expr.GetValue());
		ResolveLocal(expr, expr.GetName());
		return nullptr;
	}

	std::any VisitBinaryExpr(const Binary& expr) override
	{
		Resolve(expr.GetLeftExpr());
		Resolve(expr.GetRightExpr());
		return nullptr;
	}

	std::any VisitCallExpr(const Call& expr) override
	{
		Resolve(expr.GetCallee());

		for(auto& a : expr.GetArguments())
		{
			Resolve(a);
		}

		return nullptr;
	}

	std::any VisitGroupingExpr(const Grouping& expr) override
	{
		Resolve(expr.GetExpr());
		return nullptr;
	}

	std::any VisitLiteralExpr(const Literal& expr) override
	{
		return nullptr;
	}

	std::any VisitLogicalExpr(const Logical& expr) override
	{
		Resolve(expr.GetLeft());
		Resolve(expr.GetRight());
		return nullptr;
	}

	std::any VisitUnaryExpr(const Unary& expr) override
	{
		Resolve(expr.GetRight());
		return nullptr;
	}

	std::any VisitVariableExpr(const Var& expr) override
	{
		if (!_scopes.empty() &&
			_scopes[_scopes.size() - 1][expr.GetName().GetLexeme()] == false)
		{
			Lox::Error(expr.GetName(), "Can't read local variable in its own initializer.");
		}

		ResolveLocal(expr, expr.GetName());
	}

private:
	void BeginScope()
	{
		_scopes.push_back(std::map<std::string, bool>());
	}

	void Resolve(std::vector<StmtPtr> statements)
	{
		for (auto& s : statements)
		{
			Resolve(s);
		}
	}

	void Resolve(StmtPtr statement)
	{
		statement->Accept(*this);
	}

	void Resolve(ExprPtr expression)
	{
		expression->Accept(*this);
	}

	void ResolveLocal(const Expr& expr, Token name)
	{
		for (int i = _scopes.size() - 1; i >= 0; i--)
		{
			if (_scopes[i].find(name.GetLexeme()) != _scopes[i].end())
			{
				_interpreter->Resolve(expr, std::size(_scopes) - 1 - i);
			}
		}
	}

	void ResolveFunction(const Function& function)
	{
		BeginScope();
		for (auto& t : function.GetParams())
		{
			Declare(t);
			Define(t);
		}
		Resolve(function.GetBody());
		EndScope();
	}

	void EndScope()
	{
		_scopes.erase(_scopes.begin() + _scopes.size() - 1);
	}

	void Declare(Token name)
	{
		if (_scopes.empty())
		{
			return;
		}

		std::map<std::string, bool>& scope = _scopes[std::size(_scopes) - 1];
		scope[name.GetLexeme()] = false;
	}

	void Define(Token name)
	{
		if (_scopes.empty())
		{
			return;
		}

		std::map<std::string, bool>& scope = _scopes[std::size(_scopes) - 1];
		scope[name.GetLexeme()] = true;
	}
};