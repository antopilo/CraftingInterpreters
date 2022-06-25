#include "Lox.h"
#include "Scanner.h"

#include <iostream>
#include <fstream>
#include <string>

#include "Expr/ExprVisitor.h"

#include "AstPrinter.h"
#include "Expr/Expr.h"

void Run(const std::string& source)
{
	Scanner scanner = Scanner(source);
	std::vector<Token> tokens = scanner.ScanTokens();

	for (const Token& token : tokens)
	{
		std::cout << token.ToString() << std::endl;
	}
}

void RunFile(const std::string& file)
{
	std::ifstream ifs;
	std::string fileContent;
	std::string currenLine;

	ifs.open(file, std::ios::in);
	if (ifs)
	{
		while (!ifs.eof())
		{
			std::getline(ifs, currenLine);
			fileContent += currenLine;
		}

		ifs.close();
	}

	Run(fileContent);
}

void RunPrompt()
{
	std::string input;
	
	for (;;)
	{
		std::cout << "> ";
		std::getline(std::cin, input);

		if (input == "")
		{
			break;
		}

		Run(input);
		Lox::HadError = false;
	}
}


#include <memory>
#include "Memory.h"

int main(int argc, char *argv[])
{
	auto expression = CreateRef<Binary>(
		CreateRef<Unary>(
			Token(TokenType::MINUS, "-", "", 1),
			CreateRef<Literal>(123.0)),
		Token(TokenType::STAR, "*", "", 1),
		CreateRef<Grouping>(
			CreateRef<Literal>(45.75))
	);


	ExprPtr op = CreateRef<Literal>(123);
	auto token = Token(TokenType::MINUS, "-", "", 1);
	auto left = CreateRef<Unary>(token, std::move(op));

	AstPrinter printer;
	std::cout << std::any_cast<std::string>(printer.Print(expression));

	if (argc > 2)
	{
		std::cout << "Usage: lox [script]" << std::endl;
		return -1;
	} 
	else if (argc == 2)
	{
		RunFile(argv[1]);
		
		if(Lox::HadError)
		{
			return 65;
		}
	}
	else
	{
		RunPrompt();
	}
}