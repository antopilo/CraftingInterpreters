#include "Lox.h"
#include "Scanner.h"

#include <iostream>
#include <fstream>
#include <string>

#include "Expr/ExprVisitor.h"
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
int main(int argc, char *argv[])
{
	//auto expression = std::make_unique<Binary>(
	//	std::make_unique<Unary>(
	//		Token(TokenType::MINUS, "-", "", 1),
	//		std::make_unique<Literal>(123)),
	//	Token(TokenType::STAR, "*", "", 1),
	//	std::make_unique<Grouping>(
	//		std::make_unique<Literal>(45.75))
	//	);

	//AstPrinter printer;
	//std::unique_ptr<Expr> ptr = std::unique_ptr<Expr>(expression.get());
	//std::cout << printer.Print(std::move(ptr));

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