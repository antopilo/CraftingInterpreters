#pragma once
#include "Expr.h"
#include "../Token.h"
#include <vector>

class AssignExpr : Expr
{
private:
	Token _Name;
	ExprPtr _Value;
public:
	AssignExpr(Token name, ExprPtr value);
	std::any Accept(ExprVisitor<std::any>& visitor) const override;
};

class BinaryExpr : Expr
{
private:
	ExprPtr _Left;
	Token _Op;
	ExprPtr _Right;

public:
	BinaryExpr(ExprPtr left, Token op, ExprPtr right);
	std::any Accept(ExprVisitor<std::any>& visitor) const override;

	const Expr& GetLeftExpr() const { return *_Left; }
	const Expr& GetRightExpr() const { return *_Right; }
	const Token& GetOp() const { return _Op; }
};

class CallExpr : Expr
{
private:
	ExprPtr _Callee;
	Token _Paren;
	std::vector<ExprPtr> _Arguments;

public:
	CallExpr(ExprPtr callee, Token paren, std::vector<ExprPtr> arguments);
	std::any Accept(ExprVisitor<std::any>& visitor) const override;
};

class GetExpr : Expr
{
private:
	ExprPtr _Object;
	Token _Name;

public:
	GetExpr(ExprPtr object, Token name);
	std::any Accept(ExprVisitor<std::any>& visitor) const override;
};

