#include "Lox.h"
#include "Scanner.h"

#include <iostream>
#include <fstream>
#include <string>

void Run(const std::string& source)
{
	Scanner scanner = Scanner(source);
	std::vector<Token> tokens = scanner.ScanTokens();

	for (Token& token : tokens)
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