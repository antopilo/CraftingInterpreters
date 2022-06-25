#pragma once
#include "Expr/ExprVisitor.h"
#include "Expr/Expr.h"

#include <vector>
#include <string>

class AstPrinter : ExprVisitor<std::string>
{
private:
    std::string Parenthesize(std::string name, std::vector<const Expr&> exprs)
    {
        std::string builder = "";

        builder += "(" + name;

        for (auto& e : exprs)
        {
            builder += " ";
            builder += std::any_cast<std::string>(e.Accept(reinterpret_cast<ExprVisitor<std::any>&>(*this)));
        }

        builder += ")";

        return builder;
    }
public:

    std::string VisitBinaryExpr(const Binary& expr) override
    {
        std::vector<const Expr&> ptrs;
        ptrs.push_back(expr.GetLeftExpr());
        ptrs.push_back(expr.GetRightExpr());

        return Parenthesize(expr.GetOp().LiteralToString(), ptrs);
    }

    std::string VisitGroupingExpr(const Grouping& expr) override
    {
        std::vector<const Expr&> ptrs;
        ptrs.push_back(expr.GetExpr());
        return Parenthesize("group", ptrs);
    }

    std::string VisitLiteralExpr(const Literal& expr) override
    {
        std::any literal = expr.GetLiteral();
        if (!literal.has_value())
        {
            return "nil";
        }

        if (literal.type().name() == "string")
        {
            return std::any_cast<std::string>(literal);
        }
            
        return std::to_string(std::any_cast<double>(literal));
    }

    std::string VisitUnaryExpr(const Unary& expr) override
    {
        std::vector<const Expr&> ptrs;
        ptrs.push_back(expr.GetRight());
        return Parenthesize(expr.GetOperator().LiteralToString(), ptrs);
    }

    std::string Print(ExprPtr expr)
    {
        std::any re = expr->Accept(reinterpret_cast<ExprVisitor<std::any>&>(*this));
        return std::any_cast<std::string>(re);
    }
};