#include "Logger.h"

void Logger::printLaunchParams(bool debug, const std::string& filePath) const
{
	if (!debugMode)
	{
		return;
	}
	std::cout << "Debug mode: " << (debug ? "ON" : "OFF") << std::endl;
	std::cout << "File path: " << filePath << std::endl;
}

void Logger::printTokens(const std::vector<Token>& tokens) const
{
	if (!debugMode)
	{
		return;
	}
	std::cout << "Tokens Count: " << tokens.size() << std::endl;
	for (const auto& token : tokens)
	{
		std::cout << token.toString() << ", ";
	}
	std::cout << std::endl;
}

void Logger::printHeader(const std::string& header) const
{
	if (!debugMode)
	{
		return;
	}
	std::cout << std::endl << "======== " << header << " =======" << std::endl;
}

void Logger::printErrors(const std::vector<std::string>& errors) const
{
	std::cout << "Errors Count: " << errors.size() << std::endl;
	for (const auto& error : errors)
	{
		std::cout << error << std::endl;
	}
}

void Logger::printError(const std::string& error) const
{
	std::cout << "Error: " << error << std::endl;
}

void Logger::printNoErrors() const
{
	if (!debugMode)
	{
		return;
	}
	std::cout << "No errors found" << std::endl;
}

void Logger::printCode(const std::string& code) const
{
	if (!debugMode)
	{
		return;
	}
	std::cout << std::endl << "Generated code: " << std::endl << code << std::endl;
}

void Logger::printDone() const
{
	std::cout << "Done!" << std::endl;
}
