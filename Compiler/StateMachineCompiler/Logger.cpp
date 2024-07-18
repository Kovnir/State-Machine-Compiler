#include "Logger.h"

void Logger::printLaunchParams(bool debug, const string& filePath) const
{
	if (!debugMode)
	{
		return;
	}
	cout << "Debug mode: " << (debug ? "ON" : "OFF") << endl;
	cout << "File path: " << filePath << endl;
}

void Logger::printInput(const string& input) const
{
	if (!debugMode)
	{
		return;
	}
	cout << "Input: " << endl << input << endl;
}

void Logger::printTokens(const vector<Token>& tokens) const
{
	if (!debugMode)
	{
		return;
	}
	cout << "Tokens Count: " << tokens.size() << endl;
	for (const auto& token : tokens)
	{
		cout << token.toString() << ", ";
	}
	cout << endl;
}

void Logger::printHeader(const string& header) const
{
	if (!debugMode)
	{
		return;
	}
	cout << endl << "======== " << header << " =======" << endl;
}

void Logger::printErrors(const vector<string>& errors) const
{
	cout << "Errors Count: " << errors.size() << endl;
	for (const auto& error : errors)
	{
		cout << error << endl;
	}
}

void Logger::printError(const string& error) const
{
	cout << "Error: " << error << endl;
}

void Logger::printNoErrors() const
{
	if (!debugMode)
	{
		return;
	}
	cout << "No errors found" << endl;
}

void Logger::printCode(const string& code) const
{
	if (!debugMode)
	{
		return;
	}
	cout << endl << "Generated code: " << endl << code << endl;
}

void Logger::printDone() const
{
	cout << "Done!" << endl;
}
