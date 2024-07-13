#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include "Token.cpp"
#include "Tokenizer.h"
#include "Optimizer.h"
#include "SyntaxTree.h"
#include "ErrorChecker.h"
#include "Logger.h"
#include "CodeGenerator.h"
#include "FileProcessor.cpp"

bool parseArguments(int argc, char* argv[], bool& debug, std::string& filePath) 
{
	debug = false;
	filePath.clear();

	if (argc < 2 || argc > 3) {
		return false;
	}

	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		if (arg == "--debug") {
			debug = true;
		}
		else {
			filePath = arg;
		}
	}

	return true;
}


int main(int argc, char* argv[])
{
	bool debug = false;
	std::string filePath;

	if (!parseArguments(argc, argv, debug, filePath)) {
		std::cerr << "Usage: " << argv[0] << " [--debug] <path_to_file>" << std::endl;
		return 1;
	}

	auto logger = Logger(debug);
	auto fileProcessor = FileProcessor(false, logger);

	logger.printLaunchParams(debug, filePath);

	std::string input = fileProcessor.readFile(filePath);

	if (input.empty()) {
		logger.printError("Failed to read the file");
		return 1;
	}
	
	std::vector<Token> tokens = Tokenizer::parse(input);

	logger.printTokens(tokens);

	/*-----------------------------------------------*/

	logger.printHeader("clearEverythingBeforeAndAfterStarSlash");
	tokens = Optimizer::clearEverythingBeforeAndAfterStarSlash(tokens);
	logger.printTokens(tokens);

	logger.printHeader("clearEverythingBeforeStar");
	tokens = Optimizer::clearEverythingBeforeStar(tokens);
	logger.printTokens(tokens);

	logger.printHeader("clearDoubleNewLines");
	tokens = Optimizer::clearDoubleNewLines(tokens);
	logger.printTokens(tokens);

	logger.printHeader("removeAllSpaces");
	tokens = Optimizer::clearAllSpaces(tokens);
	logger.printTokens(tokens);

	logger.printHeader("clearAllComments");
	tokens = Optimizer::clearAllComments(tokens);
	logger.printTokens(tokens);

	logger.printHeader("clearNewLinesOnStart");
	tokens = Optimizer::clearNewLinesOnStart(tokens);
	logger.printTokens(tokens);

	logger.printHeader("ClearNewLinesOnEnd");
	tokens = Optimizer::clearNewLinesOnEnd(tokens);
	logger.printTokens(tokens);


	/*-----------------------------------------------*/


	std::vector<std::string> errors = ErrorChecker::checkErrors(tokens);

	if (!errors.empty())
	{
		logger.printErrors(errors);
		return 1;
	}
	else
	{
		logger.printNoErrors();
	}


	/*-----------------------------------------------*/

	SyntaxTree syntaxTree;
	std::string error = syntaxTree.parseTokens(tokens);

	if (!error.empty())
	{
		logger.printError(error);
		return 1;
	}

	StateMachine* root = syntaxTree.root.get();
	std::string code;
	try
	{
		code = CodeGenerator::generate(*root);
	}
	catch (const std::exception& e)
	{
		logger.printError(e.what());
		return 1;
	}

	logger.printCode(code);

	if (!fileProcessor.writeToFile(filePath, input, code))
	{
		logger.printError("Failed to write to the file");
		return 1;
	}

	logger.printDone();

	return 0;
}

