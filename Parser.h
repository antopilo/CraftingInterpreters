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

public:
	Parser(std::vector<Token> tokens);
	~Parser() = default;

	std::vector<StmtPtr> Parse();
private:
	void Synchronize();

	StmtPtr Declaration();
	StmtPtr VarDeclaration();
	StmtPtr Statement();
	StmtPtr PrintStatement();
	StmtPtr ExpressionStatement();

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
	ExprPtr Equality();
	ExprPtr Comparison();

};