#pragma once
#include <vector>
#include "Token.cpp"

class Tokenizer
{
public:
	// Disallow creating an instance of this object
	Tokenizer() = delete;
	static std::vector<Token> parse(std::string input);

private:
	static bool isBlankNotNew(char c);
	static bool isBlank(char c);
};
