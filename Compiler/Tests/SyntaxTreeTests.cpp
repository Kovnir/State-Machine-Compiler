#include "pch.h"
#include <gtest/gtest.h>
#include "../StateMachineCompiler/SyntaxTree.cpp"


TEST(SyntaxTree, parseTokens)
{

	// from this code
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
	 // 
	 //we have new tokens:
	 // TestStateMachine-NewLine-                       //State Machine Definition
	 // IdleState-default-NewLine-                      //State Definition
	 // on-Play-Arrow-RunningState-NewLine-             //Transition Definition
	 // RunningState-NewLine-							//State Definition
	 // on-Pause-Arrow-PausedState-NewLine-				//Transition Definition
	 // on-Finish-Arrow-FinishedState-NewLine-			//Transition Definition
	 // on-Stop-Arrow-IdleState-NewLine-				//Transition Definition
	 // PausedState-NewLine-                            //State Definition
	 // on-Resume-Arrow-RunningState-NewLine-			//Transition Definition
	 // on-Stop-Arrow-IdleState-NewLine-				//Transition Definition
	 // FinishedState-NewLine-                          //State Definition
	 // on-Replay-Arrow-RunningState					//Transition Definition

	std::vector<Token> tokens = {
		Token(TokenType::CUSTOM_NAME, "TestStateMachine", 1, 3),
		Token(TokenType::NEW_LINE, "", 1, 19),

		Token(TokenType::CUSTOM_NAME, "IdleState", 2, 4),
		Token(TokenType::DEFAULT, "", 2, 13),
		Token(TokenType::NEW_LINE, "", 2, 14),

		Token(TokenType::ON, "", 3, 0),
		Token(TokenType::CUSTOM_NAME, "Play", 3, 3),
		Token(TokenType::ARROW, "", 3, 7),
		Token(TokenType::CUSTOM_NAME, "RunningState", 3, 10),
		Token(TokenType::NEW_LINE, "", 3, 22),

		Token(TokenType::CUSTOM_NAME, "RunningState", 4, 4),
		Token(TokenType::NEW_LINE, "", 4, 15),

		Token(TokenType::ON, "", 5, 0),
		Token(TokenType::CUSTOM_NAME, "Pause", 5, 3),
		Token(TokenType::ARROW, "", 5, 8),
		Token(TokenType::CUSTOM_NAME, "PausedState", 5, 11),
		Token(TokenType::NEW_LINE, "", 5, 22),

		Token(TokenType::ON, "", 6, 0),
		Token(TokenType::CUSTOM_NAME, "Finish", 6, 3),
		Token(TokenType::ARROW, "", 6, 9),
		Token(TokenType::CUSTOM_NAME, "FinishedState", 6, 12),
		Token(TokenType::NEW_LINE, "", 6, 26),

		Token(TokenType::ON, "", 7, 0),
		Token(TokenType::CUSTOM_NAME, "Stop", 7, 3),
		Token(TokenType::ARROW, "", 7, 7),
		Token(TokenType::CUSTOM_NAME, "IdleState", 7, 12),
		Token(TokenType::NEW_LINE, "", 7, 21),

		Token(TokenType::CUSTOM_NAME, "PausedState", 8, 4),
		Token(TokenType::NEW_LINE, "", 8, 15),

		Token(TokenType::ON, "", 9, 0),
		Token(TokenType::CUSTOM_NAME, "Resume", 9, 3),
		Token(TokenType::ARROW, "", 9, 9),
		Token(TokenType::CUSTOM_NAME, "RunningState", 9, 12),
		Token(TokenType::NEW_LINE, "", 9, 24),

		Token(TokenType::ON, "", 10, 0),
		Token(TokenType::CUSTOM_NAME, "Stop", 10, 3),
		Token(TokenType::ARROW, "", 10, 7),
		Token(TokenType::CUSTOM_NAME, "IdleState", 10, 12),
		Token(TokenType::NEW_LINE, "", 10, 21),

		Token(TokenType::CUSTOM_NAME, "FinishedState", 11, 4),
		Token(TokenType::NEW_LINE, "", 11, 17),

		Token(TokenType::ON, "", 12, 0),
		Token(TokenType::CUSTOM_NAME, "Replay", 12, 3),
		Token(TokenType::ARROW, "", 12, 9),
		Token(TokenType::CUSTOM_NAME, "RunningState", 12, 12)
	};


	SyntaxTree syntaxTree;
	std::string error = syntaxTree.parseTokens(tokens);

//	EXPECT_EQ(error, "");
}