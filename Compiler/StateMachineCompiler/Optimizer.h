#pragma once
#include <vector>
#include "Token.cpp"

class Optimizer
{
public:
	// Disallow creating an instance of this object
	Optimizer() = delete;

	static std::vector<Token> clearEverythingBeforeAndAfterStarSlash(std::vector<Token> tokens);
	static std::vector<Token> clearEverythingBeforeStar(std::vector<Token> tokens);
	static std::vector<Token> clearNewLinesOnStart(std::vector<Token> tokens);
	static std::vector<Token> clearNewLinesOnEnd(std::vector<Token> tokens);
	static std::vector<Token> clearDoubleNewLines(std::vector<Token> tokens);
	static std::vector<Token> clearAllSpaces(std::vector<Token> tokens);
	static std::vector<Token> clearAllComments(std::vector<Token> tokens);
};
