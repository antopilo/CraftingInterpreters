#pragma once
#include "Expr/ExprVisitor.h"
#include "StmtVisitor.h"

#include "Expr/Expr.h"
#include "Stmt.h"

#include "Callable.h"
#include "Environment.h"
#include "Memory.h"
#include "Token.h"

#include <any>

class Interpreter : public ExprVisitor<std::any>, public StmtVisitor<std::any>
{
private:
	Ref<Environment> _globals;
	Environment* _environment;
	Environment* _globalEnvironmnent;
public:
	Interpreter()
	{
		_globals = CreateRef<Environment>();
		_globals->Define("clock", CreateRef<Clock>());
		_globalEnvironmnent = _globals.get();

		_environment = _globals.get();
	}
	~Interpreter() = default;

	void Interpret(std::vector<StmtPtr> expression);

	std::any VisitBinaryExpr(const Binary& expr) override;
	std::any VisitCallExpr(const Call& expr) override;
	std::any VisitGroupingExpr(const Grouping& expr) override;
	std::any VisitLiteralExpr(const Literal& expr) override;
	std::any VisitLogicalExpr(const Logical& expr) override;
	std::any VisitUnaryExpr(const Unary& expr) override;
	std::any VisitVariableExpr(const Var& expr) override;

	std::any VisitAssignExpr(const Assign& stmt) override;
	std::any VisitBlockStmt(const BlockStmt& stmt) override;
	std::any VisitExpressionStmt(const ExpressionStmt& stmt) override;
	std::any VisitFunctionStmt(const Function& stmt) override;
	std::any VisitIfStmt(const If& stmt) override;
	std::any VisitPrintStmt(const PrintStmt& stmt) override;
	std::any VisitReturnStmt(const ReturnStmt& stnt) override;
	std::any VisitVarStmt(const VarStmt& stmt) override;
	std::any VisitWhileStmt(const While& stmt) override;

	Environment* GetGlobalEnvironment() const { return _globals.get(); }

	void ExecuteBlock(std::vector<StmtPtr> statements, Environment& env);

private:
	void Execute(StmtPtr stmt);
	std::string Stringify(std::any object);
	std::any Evaluate(ExprPtr expr);
	bool IsEqual(std::any left, std::any right);
	void CheckNumberOperand(Token op, std::any operand);
	void CheckNumberOperands(Token op, std::any left, std::any right);
	bool IsTruthy(std::any object);

};