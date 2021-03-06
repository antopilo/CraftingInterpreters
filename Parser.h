#pragma once
#include "Token.h"
#include "Expr/Expr.h"
#include "Stmt.h"

#include <exception>
#include <vector>

class ParseError : public std::exception
{};

class Parser
{
private:
	std::vector<Token> _tokens;
	uint32_t _current = 0;
	const uint32_t MAX_ARGUMENTS = 255;

public:
	Parser(std::vector<Token> tokens);
	~Parser() = default;

	std::vector<StmtPtr> Parse();
private:
	void Synchronize();

	StmtPtr Declaration();
	StmtPtr VarDeclaration();
	StmtPtr Statement();
	StmtPtr ForStatement();
	StmtPtr ReturnStatement();
	StmtPtr IfStatement();
	StmtPtr PrintStatement();
	StmtPtr WhileStatement();
	StmtPtr ExpressionStatement();
	StmtPtr FunctionStatement(std::string kind);
	std::vector<StmtPtr> Block();

	bool Match(std::vector<TokenType> types);
	bool Check(TokenType type);
	Token Advance();
	bool IsAtEnd();
	Token Peek();
	Token Previous();
	ParseError Error(Token token, std::string message);
	Token Consume(TokenType type, std::string message);

	ExprPtr Term();
	ExprPtr Factor();
	ExprPtr Unary_();
	ExprPtr Primary();
	ExprPtr Expression();
	ExprPtr Assignment();
	ExprPtr Or();
	ExprPtr And();
	ExprPtr Equality();
	ExprPtr Comparison();
	ExprPtr CallExpr();
	ExprPtr FinishCall(ExprPtr callee);

};