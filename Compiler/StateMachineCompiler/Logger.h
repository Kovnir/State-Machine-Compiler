#pragma once
#include <iostream>
#include <vector>
#include "Token.cpp"

class Logger
{
private:
	bool debugMode = false;

public:
	explicit Logger(bool debugMode) : debugMode(debugMode) {}
	void printLaunchParams(bool debug, const std::string& filePath) const;
	void printInput(const std::string& input) const;
	void printTokens(const std::vector<Token>& tokens) const;
	void printHeader(const std::string& header) const;
	void printErrors(const std::vector<std::string>& errors) const;
	void printError(const std::string& error) const;
	void printNoErrors() const;
	void printCode(const std::string& code) const;
	void printDone() const;
};

