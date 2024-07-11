#pragma once
#include <vector>
#include "Token.cpp"
#include <stdexcept>

class TokenSequence
{
	const std::vector<Token>& tokens;
	int index = 0;

public:
	explicit TokenSequence(const std::vector<Token>& tokens) : tokens(tokens) {}

	const void moveNext()
	{
		index++;
	}

	const Token* peek() const
	{
		if (index >= tokens.size())
		{
			return nullptr;
		}
		return &tokens[index];
	}

	bool hasNext() const
	{
		return index < tokens.size();
	}
};