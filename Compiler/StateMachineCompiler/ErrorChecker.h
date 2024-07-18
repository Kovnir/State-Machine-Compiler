#pragma once
#include <vector>
#include <string>
#include "Token.cpp"

class ErrorChecker
{
public:
	// Disallow creating an instance of this object
	ErrorChecker() = delete;
	static vector<string> checkErrors(const vector<Token>& tokens);
private:
	static void addError(const Token& tokens, vector<string>& errors);
};

