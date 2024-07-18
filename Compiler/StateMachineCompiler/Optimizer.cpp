#include "Optimizer.h"


//if definition started from /* we need to remove everything before /* and after */
vector<Token> Optimizer::clearEverythingBeforeAndAfterStarSlash(vector<Token> tokens)
{
	vector<Token> optimized;
	auto size = (int)tokens.size();

	int startPosition = -1;
	int endPosition = -1;

	for (int i = 0; i < size; i++)
	{
		if (tokens[i].type == TokenType::SLASH_STAR) // /*
		{
			if (startPosition == -1)
			{
				startPosition = i;
			}
		}

		if (tokens[i].type == TokenType::STAR_SLASH) // */
		{
			endPosition = i;
			break;
		}
	}

	if (startPosition != -1 && endPosition != -1)
	{
		optimized.insert(optimized.end(), tokens.begin() + startPosition + 1, tokens.begin() + endPosition);
	}
	else
	{
		optimized = tokens;
	}

	return optimized;
}


vector<Token> Optimizer::clearEverythingBeforeStar(vector<Token> tokens)
{
	vector<Token> optimized;
	auto size = (int)tokens.size();
	vector<Token> line;

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



// if there are a few new line before code starts - remove them
vector<Token> Optimizer::clearNewLinesOnStart(vector<Token> tokens)
{
	vector<Token> optimized;
	auto size = (int)tokens.size();

	vector<Token> line;


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

// if there are a few new line after code ends - remove them
vector<Token> Optimizer::clearNewLinesOnEnd(vector<Token> tokens)
{
	vector<Token> optimized;
	auto size = (int)tokens.size();

	vector<Token> line;


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

vector<Token> Optimizer::clearDoubleNewLines(vector<Token> tokens)
{
	vector<Token> optimized;
	auto size = (int)tokens.size();

	for (int i = 0; i < size; i++)
	{
		if (tokens[i].type == TokenType::NEW_LINE)
		{
			int nextId = i + 1;
			if (nextId < size && tokens[nextId].type == TokenType::NEW_LINE)
			{
				continue;
			}
		}

		optimized.push_back(tokens[i]);
	}

	return optimized;
}

vector<Token> Optimizer::clearAllSpaces(vector<Token> tokens)
{
	vector<Token> optimized;
	auto size = (int)tokens.size();

	for (int i = 0; i < size; i++)
	{
		if (tokens[i].type != TokenType::SPACE)
		{
			optimized.push_back(tokens[i]);
		}
	}

	return optimized;
}

vector<Token> Optimizer::clearAllComments(vector<Token> tokens)
{
	vector<Token> optimized;
	auto size = (int)tokens.size();

	for (int i = 0; i < size; i++)
	{
		if (tokens[i].type != TokenType::COMMENT)
		{
			optimized.push_back(tokens[i]);
		}
	}

	return optimized;
}