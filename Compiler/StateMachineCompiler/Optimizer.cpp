#include "Optimizer.h"



std::vector<Token> Optimizer::clearEverythingBeforeStar(std::vector<Token> tokens)
{
	std::vector<Token> optimized;
	auto size = (int)tokens.size();
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

std::vector<Token> Optimizer::clearStringEndings(std::vector<Token> tokens)
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

std::vector<Token> Optimizer::clearNewLinesOnStart(std::vector<Token> tokens)
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

std::vector<Token> Optimizer::clearNewLinesOnEnd(std::vector<Token> tokens)
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


std::vector<Token> Optimizer::clearSpacesOnStart(std::vector<Token> tokens)
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

std::vector<Token> Optimizer::removeSpacesInMiddle(std::vector<Token> tokens)
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
