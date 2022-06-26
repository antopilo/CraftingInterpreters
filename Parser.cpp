#include "Parser.h"
#include "Memory.h"
#include "Lox.h"

Parser::Parser(std::vector<Token> tokens)
{
	_tokens = tokens;
}

void Parser::Synchronize()
{
	Advance();
	while (!IsAtEnd())
	{
		if (Previous().GetType() == TokenType::SEMICOLON)
		{
			return;
		}

		switch (Peek().GetType())
		{
		case TokenType::CLASS:
		case TokenType::FOR:
		case TokenType::FUN:
		case TokenType::IF:
		case TokenType::PRINT:
		case TokenType::RETURN:
		case TokenType::VAR:
		case TokenType::WHILE:
			return;
		}

		Advance();
	}
}

std::vector<StmtPtr> Parser::Parse()
{
	std::vector<StmtPtr> statements = std::vector<StmtPtr>();

	while (!IsAtEnd())
	{
		statements.push_back(Declaration());
	}

	return statements;
}

StmtPtr Parser::Declaration()
{
	try
	{
		if (Match({ TokenType::VAR }))
		{
			return VarDeclaration();
		}

		return Statement();
	}
	catch (ParseError error)
	{
		Synchronize();
		return nullptr;
	}
}

StmtPtr Parser::VarDeclaration()
{
	Token name = Consume(TokenType::IDENTIFIER, "Expect variable name.");

	ExprPtr initializer = nullptr;
	if (Match({ TokenType::EQUAL }))
	{
		initializer = Expression();
	}

	Consume(TokenType::SEMICOLON, "Expect ',' after variable declaration.");
	return CreateRef<VarStmt>(name, initializer);
}

StmtPtr Parser::Statement()
{
	if (Match({ TokenType::PRINT }))
	{
		return PrintStatement();
	}


	return ExpressionStatement();
}

StmtPtr Parser::PrintStatement()
{
	ExprPtr value = Expression();
	Consume(TokenType::SEMICOLON, "Expect ';' after value.");
	return CreateRef<PrintStmt>(value);
}

StmtPtr Parser::ExpressionStatement()
{
	ExprPtr value = Expression();
	Consume(TokenType::SEMICOLON, "Expect ';' after value.");
	return CreateRef<ExpressionStmt>(value);
}

bool Parser::Match(std::vector<TokenType> types)
{
	for (auto& t : types)
	{
		if (Check(t))
		{
			Advance();
			return true;
		}
	}

	return false;
}

bool Parser::Check(TokenType type)
{
	if (IsAtEnd())
	{
		return false;
	}

	return Peek().GetType() == type;
}

Token Parser::Advance()
{
	if (!IsAtEnd())
	{
		_current++;
	}

	return Previous();
}

bool Parser::IsAtEnd()
{
	return Peek().GetType() == TokenType::TokenEOF;
}

Token Parser::Peek()
{
	return _tokens.at(_current);
}

Token Parser::Previous()
{
	return _tokens.at(_current - 1);
}

ExprPtr Parser::Term()
{
	ExprPtr expr = Factor();

	while (Match({TokenType::MINUS, TokenType::PLUS}))
	{
		Token op = Previous();
		ExprPtr right = Factor();
		expr = CreateRef<Binary>(expr, op, right);
	}

	return expr;
}

ExprPtr Parser::Factor()
{
	ExprPtr expr = Unary_();

	while (Match({ TokenType::SLASH, TokenType::STAR }))
	{
		Token op = Previous();
		ExprPtr right = Unary_();
		expr = CreateRef<Binary>(expr, op, right);
	}

	return expr;
}

ExprPtr Parser::Unary_()
{
	if (Match({ TokenType::BANG, TokenType::MINUS }))
	{
		Token op = Previous();
		ExprPtr right = Unary_();
		return CreateRef<Unary>(op, right);
	}

	return Primary();
}

ExprPtr Parser::Primary()
{
	if (Match({TokenType::FALSE}))
	{
		return CreateRef<Literal>(false);
	}

	if (Match({ TokenType::TRUE	 }))
	{
		return CreateRef<Literal>(true);
	}

	if (Match({ TokenType::NIL }))
	{
		return CreateRef<Literal>(nullptr);
	}

	if (Match({ TokenType::NUMBER, TokenType::STRING }))
	{
		return CreateRef<Literal>(Previous().GetLiteral());
	}

	if (Match({ TokenType::IDENTIFIER }))
	{
		return CreateRef<Var>(Previous());
	}

	if (Match({TokenType::LEFT_PAREN}))
	{
		ExprPtr expr = Expression();
		Consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
		return CreateRef<Grouping>(expr);
	}

	throw Error(Peek(), "Expect expression.");
}

Token Parser::Consume(TokenType type, std::string message)
{
	if (Check(type))
	{
		return Advance();
	}

	throw Error(Peek(), message);
}

ParseError Parser::Error(Token token, std::string message)
{
	Lox::Error(token, message);
	return ParseError();
}

ExprPtr Parser::Expression()
{
	return Equality();
}

ExprPtr Parser::Equality()
{
	ExprPtr expr = Comparison();
	while (Match({ TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL }))
	{
		Token op = Previous();
		ExprPtr right = Comparison();
		expr = CreateRef<Binary>(expr, op, right);
	}

	return expr;
}

ExprPtr Parser::Comparison()
{
	ExprPtr expr = Term();

	while (Match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType:: LESS, TokenType::LESS_EQUAL}))
	{
		Token op = Previous();
		ExprPtr right = Term();
		expr = CreateRef<Binary>(expr, op, right);
	}

	return expr;
}
