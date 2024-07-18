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

	vector<Token> tokens = {
		Token(TokenType::CUSTOM_NAME, "TestStateMachine", 1, 3),
		Token(TokenType::NEW_LINE, "", 1, 19),

		Token(TokenType::CUSTOM_NAME, "IdleState", 2, 4),    //state IdleState
		Token(TokenType::DEFAULT, "", 2, 13),
		Token(TokenType::NEW_LINE, "", 2, 14),

		Token(TokenType::ON, "", 3, 0),
		Token(TokenType::CUSTOM_NAME, "Play", 3, 3),
		Token(TokenType::ARROW, "", 3, 7),
		Token(TokenType::CUSTOM_NAME, "RunningState", 3, 10),
		Token(TokenType::NEW_LINE, "", 3, 22),

		Token(TokenType::CUSTOM_NAME, "RunningState", 4, 4), //state RunningState
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

		Token(TokenType::CUSTOM_NAME, "PausedState", 8, 4), //state PausedState
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

		Token(TokenType::CUSTOM_NAME, "FinishedState", 11, 4), //state FinishedState
		Token(TokenType::NEW_LINE, "", 11, 17),

		Token(TokenType::ON, "", 12, 0),
		Token(TokenType::CUSTOM_NAME, "Replay", 12, 3),
		Token(TokenType::ARROW, "", 12, 9),
		Token(TokenType::CUSTOM_NAME, "RunningState", 12, 12)
	};


	SyntaxTree syntaxTree;
	string error = syntaxTree.parseTokens(tokens);

	EXPECT_EQ(error, "");

	//get syntaxTree.root
	StateMachine* root = syntaxTree.root.get();

	EXPECT_EQ(root->data.value, "TestStateMachine");
	EXPECT_EQ(root->states.size(), 4);

	/*-----------------------*/
	//IdleState
	State* idleState = root->states[0].get();
	EXPECT_EQ(idleState->data.value, "IdleState");
	EXPECT_EQ(idleState->isDefault, true);
	EXPECT_EQ(idleState->triggers.size(), 1);

	//on Play -> RunningState
	Trigger* trigger = idleState->triggers[0].get();
	EXPECT_EQ(trigger->condition, "Play");
	EXPECT_EQ(trigger->targetStateString, "RunningState");

	/*-----------------------*/
	//RunningState
	State* runningState = root->states[1].get();
	EXPECT_EQ(runningState->data.value, "RunningState");
	EXPECT_EQ(runningState->isDefault, false);
	EXPECT_EQ(runningState->triggers.size(), 3);

	//on Pause -> PausedState
	trigger = runningState->triggers[0].get();
	EXPECT_EQ(trigger->condition, "Pause");
	EXPECT_EQ(trigger->targetStateString, "PausedState");

	//on Finish -> FinishedState
	trigger = runningState->triggers[1].get();
	EXPECT_EQ(trigger->condition, "Finish");
	EXPECT_EQ(trigger->targetStateString, "FinishedState");

	//on Stop -> IdleState
	trigger = runningState->triggers[2].get();
	EXPECT_EQ(trigger->condition, "Stop");
	EXPECT_EQ(trigger->targetStateString, "IdleState");
	
	/*-----------------------*/
	//PausedState
	State* pausedState = root->states[2].get();
	EXPECT_EQ(pausedState->data.value, "PausedState");
	EXPECT_EQ(pausedState->isDefault, false);
	EXPECT_EQ(pausedState->triggers.size(), 2);

	//on Resume -> RunningState
	trigger = pausedState->triggers[0].get();
	EXPECT_EQ(trigger->condition, "Resume");
	EXPECT_EQ(trigger->targetStateString, "RunningState");

	//on Stop -> IdleState
	trigger = pausedState->triggers[1].get();
	EXPECT_EQ(trigger->condition, "Stop");
	EXPECT_EQ(trigger->targetStateString, "IdleState");

	/*-----------------------*/
	//FinishedState
	State* finishedState = root->states[3].get();
	EXPECT_EQ(finishedState->data.value, "FinishedState");
	EXPECT_EQ(finishedState->isDefault, false);
	EXPECT_EQ(finishedState->triggers.size(), 1);

	//on Replay -> RunningState
	trigger = finishedState->triggers[0].get();
	EXPECT_EQ(trigger->condition, "Replay");
	EXPECT_EQ(trigger->targetStateString, "RunningState");

}