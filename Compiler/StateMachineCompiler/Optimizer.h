#pragma once
#include <vector>
#include "Token.cpp"

using namespace std;

class Optimizer
{
public:
	// Disallow creating an instance of this object
	Optimizer() = delete;

	static vector<Token> clearEverythingBeforeAndAfterStarSlash(vector<Token> tokens);
	static vector<Token> clearEverythingBeforeStar(vector<Token> tokens);
	static vector<Token> clearNewLinesOnStart(vector<Token> tokens);
	static vector<Token> clearNewLinesOnEnd(vector<Token> tokens);
	static vector<Token> clearDoubleNewLines(vector<Token> tokens);
	static vector<Token> clearAllSpaces(vector<Token> tokens);
	static vector<Token> clearAllComments(vector<Token> tokens);
};
