#pragma once
#include <iostream>
#include <vector>
#include "Token.cpp"

class Logger
{
public:
	void printTokens(const std::vector<Token>& tokens) const;
	void printHeader(const std::string& header) const;
	void printErrors(const std::vector<std::string>& errors) const;
	void printError(const std::string& error) const;
	void printNoErrors() const;
	void printCode(const std::string& code) const;
	void printDone() const;

};

