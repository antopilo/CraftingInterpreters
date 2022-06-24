#include "Scanner.h"

#include <iostream>
#include <fstream>
#include <string>

bool HadError = false;
void Report(uint32_t line, const std::string& where, const std::string& message)
{
	std::cout << "[line " << line << "] Error" << where << ": " << message << std::endl;
	HadError = true;
}

void Error(uint32_t line, const std::string& message)
{
	Report(line, "", message);
}

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
		std::cin >> input;

		if (input == "")
		{
			break;
		}

		Run(input);
		HadError = false;
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
		
		if(HadError)
		{
			return 65;
		}
	}
	else
	{
		RunPrompt();
	}
}