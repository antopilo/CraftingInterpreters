#pragma once

#include "ExprVisitor.h"

#include <memory>
#include <any>

class Expr;
typedef std::unique_ptr<Expr> ExprPtr;
class Expr
{
public:
	virtual ~Expr() = default;
	virtual std::any Accept(ExprVisitor<std::any>& visitor) const = 0;
};