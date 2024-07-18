#pragma once
#include <iostream>
#include <vector>
#include "Token.cpp"

using namespace std;

class Logger
{
private:
	bool debugMode = false;

public:
	explicit Logger(bool debugMode) : debugMode(debugMode) {}
	void printLaunchParams(bool debug, const string& filePath) const;
	void printInput(const string& input) const;
	void printTokens(const vector<Token>& tokens) const;
	void printHeader(const string& header) const;
	void printErrors(const vector<string>& errors) const;
	void printError(const string& error) const;
	void printNoErrors() const;
	void printCode(const string& code) const;
	void printDone() const;
};

