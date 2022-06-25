#include "Lox.h"
#include "Scanner.h"
#include "Expr/ExprVisitor.h"
#include "AstPrinter.h"
#include "Expr/Expr.h"
#include "Parser.h"
#include "Interpreter.h"


#include <iostream>
#include <fstream>
#include <string>

void Run(const std::string& source)
{
	Scanner scanner = Scanner(source);
	Interpreter interpreter = Interpreter();

	std::vector<Token> tokens = scanner.ScanTokens();

	Parser parser = Parser(tokens);
	ExprPtr expression = parser.Parse();

	if (Lox::HadError)
	{
		return;
	}

	interpreter.Interpret(expression);
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

	if (Lox::HadError)
		return;

	if (Lox::HadRuntimeError)
		return;
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