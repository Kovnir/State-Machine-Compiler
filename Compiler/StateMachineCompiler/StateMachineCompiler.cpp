#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include "Token.cpp"
#include "Tokenizer.h"


std::vector<Token> OptimizeTokens_ClearEverythingBeforeStar(std::vector<Token> tokens)
{
	std::vector<Token> optimized;
	auto size = (int) tokens.size();
	std::vector<Token> line;

	for (int i = 0; i < size; i++)
	{
		if (tokens[i].type == TokenType::STAR_SLASH) // */ jusr skip it
		{
			continue;
		}

		if (tokens[i].type == TokenType::STAR || tokens[i].type == TokenType::SLASH_STAR) // /*
		{
			line.clear();
			continue;
		}

		line.push_back(tokens[i]);

		if (tokens[i].type == TokenType::NEW_LINE)
		{
			optimized.insert(optimized.end(), line.begin(), line.end());
			line.clear();
		}
	}

	return optimized;
}

std::vector<Token> OptimizeTokens_ClearStringEndings(std::vector<Token> tokens)
{
	std::vector<Token> optimized;
	auto size = (int)tokens.size();

	std::vector<Token> skiped;


	for (int i = 0; i < size; i++)
	{
		if (tokens[i].type == TokenType::SPACE)
		{
			skiped.push_back(tokens[i]);
		}
		else
		{
			if (tokens[i].type == TokenType::NEW_LINE)
			{
				skiped.clear();
				optimized.push_back(tokens[i]);
			}
			else
			{
				optimized.insert(optimized.end(), skiped.begin(), skiped.end());
				skiped.clear();
				optimized.push_back(tokens[i]);
			}
		}
	}

	return optimized;
}

std::vector<Token> OptimizeTokens_ClearNewLinesOnStart(std::vector<Token> tokens)
{
	std::vector<Token> optimized;
	auto size = (int)tokens.size();

	std::vector<Token> line;


	bool startFound = false;
	for (int i = 0; i < size; i++)
	{
		if (startFound)
		{
			optimized.push_back(tokens[i]);
			continue;
		}
		if (tokens[i].type != TokenType::NEW_LINE)
		{
			startFound = true;
			optimized.push_back(tokens[i]);
		}
	}

	return optimized;
}

std::vector<Token> OptimizeTokens_ClearNewLinesOnEnd(std::vector<Token> tokens)
{
	std::vector<Token> optimized;
	auto size = (int)tokens.size();

	std::vector<Token> line;


	bool newLineFound = false;
	// remove spaces before new lines
	for (int i = 0; i < size; i++)
	{
		if (tokens[i].type == TokenType::NEW_LINE)
		{
			newLineFound = true;
		}
		else
		{
			newLineFound = false;
			optimized.insert(optimized.end(), line.begin(), line.end());
			line.clear();
		}

		if (newLineFound)
		{
			line.push_back(tokens[i]);
		}
		else
		{
			optimized.push_back(tokens[i]);
		}
	}

	return optimized;
}


std::vector<Token> OptimizeTokens_ClearSpacesOnStart(std::vector<Token> tokens)
{
	std::vector<Token> optimized;
	auto size = (int)tokens.size();

	int minCount = INT_MAX;
	int currentCount = 0;


	bool startFound = false;
	// remove spaces before new lines
	for (int i = 0; i < size; i++)
	{
		if (tokens[i].type != TokenType::SPACE)
		{
			startFound = true;
		}

		if (!startFound)
		{
			currentCount++;
		}

		if (tokens[i].type == TokenType::NEW_LINE)
		{
			if (currentCount < minCount)
			{
				minCount = currentCount;
			}
			currentCount = 0;
			startFound = false;
		}
	}

	currentCount = 0;
	for (int i = 0; i < size; i++)
	{
		currentCount++;
		if (currentCount > minCount)
		{
			optimized.push_back(tokens[i]);
		}
		if (tokens[i].type == TokenType::NEW_LINE)
		{
			currentCount = 0;
		}
	}

	return optimized;
}

std::vector<Token> OptimizeTokens_RemoveSpacesInMiddle(std::vector<Token> tokens)
{
	std::vector<Token> optimized;
	auto size = (int)tokens.size();


	bool contentFound = false;
	// remove spaces before new lines
	for (int i = 0; i < size; i++)
	{
		if (tokens[i].type != TokenType::SPACE)
		{
			optimized.push_back(tokens[i]);
			contentFound = true;
		}
		else
		{
			if (!contentFound)
			{
				optimized.push_back(tokens[i]);
			}
		}

		if (tokens[i].type == TokenType::NEW_LINE)
		{
			contentFound = false;
		}
	}

	return optimized;
}

void PrintTokens(std::vector<Token> tokens);

int main()
{
	//int x = 5;
	//std::cout << "x = " + x << std::endl;
	//x++;
	//std::cout << "x = " + x << std::endl;
	//int& ref = x; // Объявление ссылки на переменную x
	//std::cout << "ref = " + ref << std::endl;
	//int* ptr = &x; // Получение адреса переменной x
	//std::cout << "*ptr = " + *ptr << std::endl;
	//std::cout << ptr << std::endl;
	//int sm = x; // Получение адреса переменной x
	//std::cout << "SM = " + sm << std::endl;

	//return 0;
	// 
	//std::string input = "\
 // /*\n\
 //   * TestStateMachine\n\
 //   *   IdleState default\n\
 //   *     on Play -> RunningState\n\
 //   *   RunningState // some comment\n\
 //   *     on Pause -> PausedState    \n\
 //   *     on Finish -> FinishedState \n\
 //   *     on Stop ->    IdleState\n\
 //   *   PausedState\n\
 //   *     on Resume -> RunningState\n\
 //   *     on Stop -> IdleState\n\
 //   *   FinishedState\n\
 //   *     on Replay -> RunningState\n\
 //   */\n\                  ";

	std::string input = R"(  /*
    * TestStateMachine
    *   IdleState default
    *     on Play -> RunningState    
    *   RunningState // some comment
    *     on Pause -> PausedState    
    */
)";

	std::vector<Token> tokens = Tokenizer::Parse(input);

	PrintTokens(tokens);

	std::cout << std::endl << "======== ClearEverythingBeforeStar =======" << std::endl;
	tokens = OptimizeTokens_ClearEverythingBeforeStar(tokens);
	PrintTokens(tokens);

	std::cout << std::endl << "======== ClearStringEndings =======" << std::endl;
	tokens = OptimizeTokens_ClearStringEndings(tokens);
	PrintTokens(tokens);

	std::cout << std::endl << "======== ClearNewLinesOnStart =======" << std::endl;
	tokens = OptimizeTokens_ClearNewLinesOnStart(tokens);
	PrintTokens(tokens);

	std::cout << std::endl << "======== ClearNewLinesOnEnd =======" << std::endl;
	tokens = OptimizeTokens_ClearNewLinesOnEnd(tokens);
	PrintTokens(tokens);

	std::cout << std::endl << "======== ClearSpacesOnStart =======" << std::endl;
	tokens = OptimizeTokens_ClearSpacesOnStart(tokens);
	PrintTokens(tokens);

	std::cout << std::endl << "======== RemoveSpacesInMiddle =======" << std::endl;
	tokens = OptimizeTokens_RemoveSpacesInMiddle(tokens);
	PrintTokens(tokens);


	return 0;
}

void PrintTokens(std::vector<Token> tokens)
{
	std::cout << "Tokens Count: " << tokens.size() << std::endl;
	for (const auto& token : tokens)
	{
		std::cout << token.toString() << ", ";
	}
	std::cout << std::endl;
}

