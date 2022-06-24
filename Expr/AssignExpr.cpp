#include "AssignExpr.h"

AssignExpr::AssignExpr(Token name, ExprPtr value) 
{
	_Name = name;
	_Value = std::move(value);
}

std::any AssignExpr::Accept(ExprVisitor<std::any>& visitor) const
{
	return visitor.VisitAssignExpr(*this);
}
