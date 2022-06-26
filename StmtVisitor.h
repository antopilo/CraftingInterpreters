#pragma once

class ExpressionStmt;
class PrintStmt;
class VarStmt;
class BlockStmt;
class If;
class While;

template<typename R>
class StmtVisitor
{
public:
    ~StmtVisitor() = default;

    virtual R VisitExpressionStmt(const ExpressionStmt& stmt) = 0;
    virtual R VisitPrintStmt(const PrintStmt& stmt) = 0;
    virtual R VisitVarStmt(const VarStmt& stmt) = 0;
    virtual R VisitBlockStmt(const BlockStmt& stmt) = 0;
    virtual R VisitIfStmt(const If& stmt) = 0;
    virtual R VisitWhileStmt(const While& stmt) = 0;
};