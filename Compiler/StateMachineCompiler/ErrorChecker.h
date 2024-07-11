#pragma once
#include <vector>
#include <string>
#include "Token.cpp"

class ErrorChecker
{
public:
	static std::vector<std::string> checkErrors(const std::vector<Token>& tokens);
private:
	static void addError(const Token& tokens, std::vector<std::string>& errors);
};

