#pragma once

class ExpressionStmt;
class PrintStmt;

template<typename R>
class StmtVisitor
{
public:
    ~StmtVisitor() = default;

    virtual R visitExpressionStmt(const ExpressionStmt& expr) = 0;
    virtual R visitPrintStmt(const PrintStmt& expr) = 0;
};