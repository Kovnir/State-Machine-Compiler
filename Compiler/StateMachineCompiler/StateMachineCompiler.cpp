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
#include "SyntaxTreeValidator.h"
#include "Logger.h"
#include <fstream>
#include <sstream>

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

bool removeBOM(std::ifstream& file) {
	char bom[3] = { 0 };
	file.read(bom, 3);

	// Check if the file starts with the UTF-8 BOM
	if (bom[0] == '\xEF' && bom[1] == '\xBB' && bom[2] == '\xBF') {
		// BOM found, no need to put back the read characters
		return true;
	}
	else {
		// No BOM found, put back the read characters
		file.seekg(0);
		return false;
	}
}

std::string readFileWithBOMCheck(const std::string& filePath) {
	std::ifstream file(filePath, std::ios::binary);
	if (!file) {
		throw std::runtime_error("Failed to open the file for reading");
	}

	removeBOM(file);

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

int main(int argc, char* argv[])
{
	//std::string input = "\
 // /*\n\
 //   * TestStateMachine\n\
 //   *   IdleState default\n\
 //   *     on Play -> RunningState\n\
 //   *   RunningState // some comment\n\
 //   *     on Pause -> PausedState    \n\
 //   *     on Finish -> FinishedState \n\
 //   *     on Stop ->    IdleState\n\
 //   *   PausedState\n\
 //   *     on Resume -> RunningState\n\
 //   *     on Stop -> IdleState\n\
 //   *   FinishedState\n\
 //   *     on Replay -> RunningState\n\
 //   */\n\                  ";


	bool debug = false;
	std::string filePath;

	if (!parseArguments(argc, argv, debug, filePath)) {
		std::cerr << "Usage: " << argv[0] << " [--debug] <path_to_file>" << std::endl;
		return 1;
	}

	auto logger = Logger(debug);

	logger.printLaunchParams(debug, filePath);

	std::string input;
	try {
		input = readFileWithBOMCheck(argv[1]);
	}
	catch (const std::exception& e) {
		logger.printError(e.what());
		return 1;
	}

	
	//
//	std::string input = R"(  /*
//    * TestStateMachine
//    *   IdleState default
//    *     on Play -> RunningState    
//    *   RunningState // some comment
//    *     on Pause -> PausedState    
//    */
//)";
	std::vector<Token> tokens = Tokenizer::parse(input);

	logger.printTokens(tokens);

	/*-----------------------------------------------*/

	logger.printHeader("clearEverythingBeforeAndAfterStarSlash");
	tokens = Optimizer::clearEverythingBeforeAndAfterStarSlash(tokens);
	logger.printTokens(tokens);

	logger.printHeader("clearEverythingBeforeStar");
	tokens = Optimizer::clearEverythingBeforeStar(tokens);
	logger.printTokens(tokens);

	logger.printHeader("clearNewLinesOnStart");
	tokens = Optimizer::clearNewLinesOnStart(tokens);
	logger.printTokens(tokens);

	logger.printHeader("clearNewLinesOnEnd");
	tokens = Optimizer::clearNewLinesOnEnd(tokens);
	logger.printTokens(tokens);

	logger.printHeader("ClearNewLinesOnEnd");
	tokens = Optimizer::clearNewLinesOnEnd(tokens);
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

	std::ofstream outputFile(filePath, std::ios::trunc);
	if (!outputFile) {
		logger.printError("Failed to open the file for writing");
		return 1;
	}

	outputFile << input << std::endl;
	outputFile << code << std::endl;

	logger.printDone();

	return 0;
}

