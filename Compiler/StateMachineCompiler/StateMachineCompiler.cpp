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



int main()
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

	std::string input = R"(  /*
    * TestStateMachine
    *   IdleState default
    *     on Play -> RunningState    
    *   RunningState // some comment
    *     on Pause -> PausedState    
    */
)";
	Logger logger = Logger(true);

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
	logger.printDone();
	return 0;
}

