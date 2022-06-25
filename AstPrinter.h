#pragma once
#include "Expr/ExprVisitor.h"
#include "Expr/Expr.h"

#include <vector>
#include <string>

class AstPrinter : ExprVisitor <std::any>
{
private:
    std::string Parenthesize(std::string name, std::vector<ExprPtr> exprs)
    {
        std::string builder = "";

        builder += "(" + name;

        for (auto e : exprs)
        {
            builder += " ";
            auto visitor = reinterpret_cast<ExprVisitor<std::any>*>(this);
            std::any accept = e->Accept(*visitor);

            if (accept.type() == typeid(builder))
                builder += std::any_cast<std::string>(accept);
            else
                builder += std::to_string(std::any_cast<double>(accept));
        }

        builder += ")";

        return builder;
    }
public:

    std::any VisitBinaryExpr(const Binary& expr) override
    {
        std::vector<ExprPtr> ptrs;
        ptrs.push_back(expr.GetLeftExpr());
        ptrs.push_back(expr.GetRightExpr());

        return Parenthesize(expr.GetOp().LiteralToString(), ptrs);
    }

    std::any VisitGroupingExpr(const Grouping& expr) override
    {
        std::vector<ExprPtr> ptrs;
        ptrs.push_back(expr.GetExpr());
        return Parenthesize("group", ptrs);
    }

    std::any VisitLiteralExpr(const Literal& expr) override
    {
        std::any literal = expr.GetLiteral();
        if (!literal.has_value())
        {
            return "nil";
        }

        std::string str;
        auto ltrType = literal.type().name();
        if (literal.type() == typeid(str))
        {
            return std::any_cast<std::string>(literal);
        }

        if (literal.type() == typeid(bool))
        {
            return std::any_cast<bool>(literal);
        }
            
        return std::any_cast<double>(literal);
    }

    std::any VisitUnaryExpr(const Unary& expr) override
    {
        std::vector<ExprPtr> ptrs;
        ptrs.push_back(expr.GetRight());
        return Parenthesize(expr.GetOperator().LiteralToString(), ptrs);
    }

    std::any Print(ExprPtr expr)
    {
        std::any re = expr->Accept(reinterpret_cast<ExprVisitor<std::any>&>(*this));
        std::string str;
        if (re.type() == typeid(str))
            return std::any_cast<std::string>(re);
        
        return std::to_string(std::any_cast<double>(re));
    }
};