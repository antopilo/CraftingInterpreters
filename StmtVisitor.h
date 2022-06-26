#pragma once

class ExpressionStmt;
class PrintStmt;
class VarStmt;

template<typename R>
class StmtVisitor
{
public:
    ~StmtVisitor() = default;

    virtual R VisitExpressionStmt(const ExpressionStmt& expr) = 0;
    virtual R VisitPrintStmt(const PrintStmt& expr) = 0;
    virtual R VisitVarStmt(const VarStmt& expr) = 0;
};