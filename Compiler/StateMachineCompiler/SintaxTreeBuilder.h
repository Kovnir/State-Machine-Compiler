#pragma once
#include <vector>
#include "Token.cpp"

class SintaxTreeBuilder
{	
public:
	// Disallow creating an instance of this object
	SintaxTreeBuilder() = delete;

	static std::vector<Token> clearEverythingBeforeStar(std::vector<Token> tokens);
};

