#include "Expr/ExprVisitor.h"

#include "Expr/Expr.h"
#include "Token.h"

#include <any>



class Interpreter : public ExprVisitor<std::any>
{
public:
	Interpreter() = default;
	~Interpreter() = default;

	void Interpret(ExprPtr expression);

	std::any VisitLiteralExpr(const Literal& expr) override;
	std::any VisitGroupingExpr(const Grouping& expr) override;
	std::any VisitUnaryExpr(const Unary& expr) override;
	std::any VisitBinaryExpr(const Binary& expr) override;

private:
	std::string Stringify(std::any object);
	std::any Evaluate(ExprPtr expr);
	bool IsEqual(std::any left, std::any right);
	void CheckNumberOperand(Token op, std::any operand);
	void CheckNumberOperands(Token op, std::any left, std::any right);
	bool IsTruthy(std::any object);

};