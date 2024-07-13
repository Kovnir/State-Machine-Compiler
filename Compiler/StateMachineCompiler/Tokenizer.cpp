#pragma once
#include "Tokenizer.h"


std::vector<Token> Tokenizer::parse(std::string input)
{
	std::vector<Token> tokens;
	std::string current = "";

	auto length = (int) input.length();

	int line = 0;
	int position = 0;

	for (int i = 0; i < length; )
	{
		char c0 = input[i];
		char c1 = (i + 1 >= length) ? ' ' : input[i + 1];
		char c2 = (i + 2 >= length) ? ' ' : input[i + 2];
		char c3 = (i + 3 >= length) ? ' ' : input[i + 3];
		char c4 = (i + 4 >= length) ? ' ' : input[i + 4];
		char c5 = (i + 5 >= length) ? ' ' : input[i + 5];
		char c6 = (i + 6 >= length) ? ' ' : input[i + 6];
		char c7 = (i + 7 >= length) ? ' ' : input[i + 7];

		if (isBlankNotNew(c0))
		{
			tokens.emplace_back(TokenType::SPACE, "", line, position);
			position++;
			i++;
			continue;
		}

		if (c0 == '*' && c1 == '/' && isBlank(c2))
		{
			tokens.emplace_back(TokenType::STAR_SLASH, "", line, position);
			position += 2;
			i += 2;
			continue;
		}

		if (c0 == '/' && c1 == '*' && isBlank(c2))
		{
			tokens.emplace_back(TokenType::SLASH_STAR, "", line, position);
			position += 2;
			i += 2;
			continue;
		}


		if (c0 == '*' && isBlank(c1))
		{
			tokens.emplace_back(TokenType::STAR, "", line, position);
			position ++;
			i++;
			continue;
		}

		if (c0 == '\n')
		{
			tokens.emplace_back(TokenType::NEW_LINE, "", line, position);
			line++;
			position = 0;
			i++;
			continue;
		}


		if (c0 == '/' && c1 == '/')
		{
			i += 2;
			std::string value = "";

			while (i < length && input[i] != '\n')
			{
				value += input[i];
				i++;
			}

			tokens.emplace_back(TokenType::COMMENT, value, line, position);
			position += (int)value.length() + 2;
			continue;
		}


		if (c0 == '-' && c1 == '>' && isBlank(c2))
		{
			tokens.emplace_back(TokenType::ARROW, "", line, position);
			i += 2;
			position += 2;
			continue;
		}

		if ((c0 == 'o' || c0 == 'O') && (c1 == 'n' || c1 == 'N') && isBlank(c2))
		{
			tokens.emplace_back(TokenType::ON, "", line, position);
			position += 2;
			i += 2;
			continue;
		}


		if ((c0 == 'd' || c0 == 'D') && c1 == 'e' && c2 == 'f' && c3 == 'a' && c4 == 'u' && c5 == 'l' && c6 == 't' && isBlank(c7))
		{
			tokens.emplace_back(TokenType::DEFAULT, "", line, position);
			position += 7;
			i += 7;
			continue;
		}

		//custom line
		std::string value = "";

		while (i < length && !isBlank(input[i]))
		{
			value += input[i];
			i++;
		}

		tokens.emplace_back(TokenType::CUSTOM_NAME, value, line, position);
		position += (int)value.length();
	}
	return tokens;
}

bool Tokenizer::isBlankNotNew(char c)
{
	return c == ' ' || c == '\t' || c == '\r';
}

bool Tokenizer::isBlank(char c)
{
	return isBlankNotNew(c) || c == '\n';
}
