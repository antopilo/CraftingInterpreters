#pragma once
#include <vector>

class Assign;
class Binary;
class Call;
class Grouping;
class Logical;
class Literal;
class Unary;
class Var;

template<typename R>
class ExprVisitor
{
public:
	~ExprVisitor() = default;

    virtual R VisitBinaryExpr(const Binary& expr) = 0;
    virtual R VisitGroupingExpr(const Grouping& expr) = 0;
    virtual R VisitLiteralExpr(const Literal& expr) = 0;
    virtual R VisitUnaryExpr(const Unary& expr) = 0;
    virtual R VisitVariableExpr(const Var& expr) = 0;
    virtual R VisitAssignExpr(const Assign& expr) = 0;
};


