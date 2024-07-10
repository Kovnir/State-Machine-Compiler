#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include "Token.cpp"
#include "Tokenizer.h"
#include "Optimizer.h"
#include "SyntaxTree.h"


void PrintTokens(std::vector<Token> tokens);

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

	std::vector<Token> tokens = Tokenizer::parse(input);

	PrintTokens(tokens);

	std::cout << std::endl << "======== clearEverythingBeforeAndAfterStarSlash =======" << std::endl;
	tokens = Optimizer::clearEverythingBeforeAndAfterStarSlash(tokens);
	PrintTokens(tokens);

	std::cout << std::endl << "======== clearEverythingBeforeStar =======" << std::endl;
	tokens = Optimizer::clearEverythingBeforeStar(tokens);
	PrintTokens(tokens);


	std::cout << std::endl << "======== clearNewLinesOnStart =======" << std::endl;
	tokens = Optimizer::clearNewLinesOnStart(tokens);
	PrintTokens(tokens);

	std::cout << std::endl << "======== clearNewLinesOnEnd =======" << std::endl;
	tokens = Optimizer::clearNewLinesOnEnd(tokens);
	PrintTokens(tokens);

	std::cout << std::endl << "======== ClearNewLinesOnEnd =======" << std::endl;
	tokens = Optimizer::clearNewLinesOnEnd(tokens);
	PrintTokens(tokens);

	std::cout << std::endl << "======== clearDoubleNewLines =======" << std::endl;
	tokens = Optimizer::clearDoubleNewLines(tokens);
	PrintTokens(tokens);

	std::cout << std::endl << "======== removeAllSpaces =======" << std::endl;
	tokens = Optimizer::clearAllSpaces(tokens);
	PrintTokens(tokens);

	std::cout << std::endl << "======== clearAllComments =======" << std::endl;
	tokens = Optimizer::clearAllComments(tokens);
	PrintTokens(tokens);


	SyntaxTree syntaxTree;
	
	for (const auto& token : tokens)
	{
		syntaxTree.addToken(token);
	}

	return 0;
}

void PrintTokens(std::vector<Token> tokens)
{
	std::cout << "Tokens Count: " << tokens.size() << std::endl;
	for (const auto& token : tokens)
	{
		std::cout << token.toString() << ", ";
	}
	std::cout << std::endl;
}

