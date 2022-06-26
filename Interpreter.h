#include "Expr/ExprVisitor.h"
#include "StmtVisitor.h"
#include "Expr/Expr.h"
#include "Stmt.h"
#include "Token.h"

#include "Environment.h"
#include <any>

class Interpreter : public ExprVisitor<std::any>, public StmtVisitor<std::any>
{
private:
	Environment _environment;

public:
	Interpreter() = default;
	~Interpreter() = default;

	void Interpret(std::vector<StmtPtr> expression);

	std::any VisitLiteralExpr(const Literal& expr) override;
	std::any VisitGroupingExpr(const Grouping& expr) override;
	std::any VisitUnaryExpr(const Unary& expr) override;
	std::any VisitBinaryExpr(const Binary& expr) override;
	std::any VisitVariableExpr(const Var& expr) override;

	std::any VisitExpressionStmt(const ExpressionStmt& stmt) override;
	std::any VisitPrintStmt(const PrintStmt& stmt) override;
	std::any VisitVarStmt(const VarStmt& stmt) override;

private:
	void Execute(StmtPtr stmt);
	std::string Stringify(std::any object);
	std::any Evaluate(ExprPtr expr);
	bool IsEqual(std::any left, std::any right);
	void CheckNumberOperand(Token op, std::any operand);
	void CheckNumberOperands(Token op, std::any left, std::any right);
	bool IsTruthy(std::any object);

};