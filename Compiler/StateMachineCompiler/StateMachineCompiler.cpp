#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include "Token.cpp"
#include "Tokenizer.h"
#include "Optimizer.h"


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

	std::cout << std::endl << "======== ClearEverythingBeforeStar =======" << std::endl;
	tokens = Optimizer::clearEverythingBeforeStar(tokens);
	PrintTokens(tokens);

	std::cout << std::endl << "======== ClearStringEndings =======" << std::endl;
	tokens = Optimizer::clearStringEndings(tokens);
	PrintTokens(tokens);

	std::cout << std::endl << "======== ClearNewLinesOnStart =======" << std::endl;
	tokens = Optimizer::clearNewLinesOnStart(tokens);
	PrintTokens(tokens);

	std::cout << std::endl << "======== ClearNewLinesOnEnd =======" << std::endl;
	tokens = Optimizer::clearNewLinesOnEnd(tokens);
	PrintTokens(tokens);

	std::cout << std::endl << "======== ClearSpacesOnStart =======" << std::endl;
	tokens = Optimizer::clearSpacesOnStart(tokens);
	PrintTokens(tokens);

	std::cout << std::endl << "======== RemoveSpacesInMiddle =======" << std::endl;
	tokens = Optimizer::removeSpacesInMiddle(tokens);
	PrintTokens(tokens);


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

