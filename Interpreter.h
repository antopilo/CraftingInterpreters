#include "Expr/ExprVisitor.h"
#include "StmtVisitor.h"

#include "Expr/Expr.h"
#include "Stmt.h"

#include "Token.h"

#include <any>



class Interpreter : public ExprVisitor<std::any>, public StmtVisitor<std::any>
{
public:
	Interpreter() = default;
	~Interpreter() = default;

	void Interpret(std::vector<StmtPtr> expression);

	std::any VisitLiteralExpr(const Literal& expr) override;
	std::any VisitGroupingExpr(const Grouping& expr) override;
	std::any VisitUnaryExpr(const Unary& expr) override;
	std::any VisitBinaryExpr(const Binary& expr) override;

	std::any visitExpressionStmt(const ExpressionStmt& stmt) override;
	std::any visitPrintStmt(const PrintStmt& stmt) override;
private:
	void Execute(StmtPtr stmt);
	std::string Stringify(std::any object);
	std::any Evaluate(ExprPtr expr);
	bool IsEqual(std::any left, std::any right);
	void CheckNumberOperand(Token op, std::any operand);
	void CheckNumberOperands(Token op, std::any left, std::any right);
	bool IsTruthy(std::any object);

};