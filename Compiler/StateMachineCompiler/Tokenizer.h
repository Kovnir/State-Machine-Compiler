#pragma once
#include <vector>
#include "Token.cpp"

class Tokenizer
{
public:
	// Disallow creating an instance of this object
	Tokenizer() = delete;
	static vector<Token> parse(string input);

private:
	static bool isBlankNotNew(char c);
	static bool isBlank(char c);
};
