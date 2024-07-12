#pragma once
#include <vector>
#include <string>
#include "Token.cpp"

class ErrorChecker
{
public:
	// Disallow creating an instance of this object
	ErrorChecker() = delete;
	static std::vector<std::string> checkErrors(const std::vector<Token>& tokens);
private:
	static void addError(const Token& tokens, std::vector<std::string>& errors);
};

