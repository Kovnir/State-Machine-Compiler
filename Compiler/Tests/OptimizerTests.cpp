#include "pch.h"
#include <gtest/gtest.h>
#include "../../Compiler/StateMachineCompiler/Optimizer.cpp"
#include "TestUtils.cpp"


TEST(OptimizerTest, clearEverythingBeforeStar)
{
	std::vector<Token> tokens = {
		Token(TokenType::SLASH_STAR, "", 0, 0),                  // -
		Token(TokenType::NEW_LINE, "", 0, 2),
		Token(TokenType::SPACE, "", 1, 0),                      // -
		Token(TokenType::STAR, "", 1, 1),                       // -
		Token(TokenType::SPACE, "", 1, 2),
		Token(TokenType::CUSTOM_NAME, "TestStateMachine", 1, 3),
		Token(TokenType::NEW_LINE, "", 1, 19),
		Token(TokenType::SPACE, "", 2, 0),                      // -
		Token(TokenType::STAR, "", 2, 1),                       // -
		Token(TokenType::SPACE, "", 2, 2),
		Token(TokenType::SPACE, "", 2, 3),
		Token(TokenType::CUSTOM_NAME, "IdleState", 2, 4),
		Token(TokenType::SPACE, "", 2, 12),
		Token(TokenType::DEFAULT, "", 2, 13),
		Token(TokenType::SPACE, "", 2, 20),
		Token(TokenType::COMMENT, " some comment  here   ", 2, 21),
		Token(TokenType::NEW_LINE, "", 2, 46),
		Token(TokenType::SPACE, "", 3, 0),                      // -
		Token(TokenType::STAR, "", 3, 1),                       // -
		Token(TokenType::SPACE, "", 3, 2),
		Token(TokenType::SPACE, "", 3, 3),
		Token(TokenType::SPACE, "", 3, 4),
		Token(TokenType::ON, "", 3, 5),
		Token(TokenType::SPACE, "", 3, 7),
		Token(TokenType::CUSTOM_NAME, "Play", 3, 8),
		Token(TokenType::SPACE, "", 3, 12),
		Token(TokenType::ARROW, "", 3, 13),
		Token(TokenType::SPACE, "", 3, 15),
		Token(TokenType::CUSTOM_NAME, "RunningState", 3, 16),
		Token(TokenType::NEW_LINE, "", 3, 28),
		Token(TokenType::STAR_SLASH, "", 4, 0)                  // -
	};

	std::vector<Token> optimized = Optimizer::clearEverythingBeforeStar(tokens);


	ASSERT_EQ(optimized.size(), 23);


	validateToken(optimized[0], TokenType::NEW_LINE, "", 0, 2);
	validateToken(optimized[1], TokenType::SPACE, "", 1, 2);
	validateToken(optimized[2], TokenType::CUSTOM_NAME, "TestStateMachine", 1, 3);
	validateToken(optimized[3], TokenType::NEW_LINE, "", 1, 19);
	validateToken(optimized[4], TokenType::SPACE, "", 2, 2);
	validateToken(optimized[5], TokenType::SPACE, "", 2, 3);
	validateToken(optimized[6], TokenType::CUSTOM_NAME, "IdleState", 2, 4);
	validateToken(optimized[7], TokenType::SPACE, "", 2, 12);
	validateToken(optimized[8], TokenType::DEFAULT, "", 2, 13);
	validateToken(optimized[9], TokenType::SPACE, "", 2, 20);
	validateToken(optimized[10], TokenType::COMMENT, " some comment  here   ", 2, 21);
	validateToken(optimized[11], TokenType::NEW_LINE, "", 2, 46);
	validateToken(optimized[12], TokenType::SPACE, "", 3, 2);
	validateToken(optimized[13], TokenType::SPACE, "", 3, 3);
	validateToken(optimized[14], TokenType::SPACE, "", 3, 4);
	validateToken(optimized[15], TokenType::ON, "", 3, 5);
	validateToken(optimized[16], TokenType::SPACE, "", 3, 7);
	validateToken(optimized[17], TokenType::CUSTOM_NAME, "Play", 3, 8);
	validateToken(optimized[18], TokenType::SPACE, "", 3, 12);
	validateToken(optimized[19], TokenType::ARROW, "", 3, 13);
	validateToken(optimized[20], TokenType::SPACE, "", 3, 15);
	validateToken(optimized[21], TokenType::CUSTOM_NAME, "RunningState", 3, 16);
	validateToken(optimized[22], TokenType::NEW_LINE, "", 3, 28);
}

TEST(OptimizerTest, clearStringEndings)
{
	std::vector<Token> tokens = {
		Token(TokenType::NEW_LINE, "", 0, 2),
		Token(TokenType::SPACE, "", 1, 2),
		Token(TokenType::CUSTOM_NAME, "TestStateMachine", 1, 3),
		Token(TokenType::SPACE, "", 1, 19),							//new
		Token(TokenType::SPACE, "", 2, 2),							//new
		Token(TokenType::NEW_LINE, "", 1, 19),
		Token(TokenType::SPACE, "", 2, 2),
		Token(TokenType::SPACE, "", 2, 3),
		Token(TokenType::CUSTOM_NAME, "IdleState", 2, 4),
		Token(TokenType::SPACE, "", 2, 12),
		Token(TokenType::DEFAULT, "", 2, 13),
		Token(TokenType::SPACE, "", 2, 20),
		Token(TokenType::COMMENT, " some comment  here   ", 2, 21),
		Token(TokenType::NEW_LINE, "", 2, 46),
		Token(TokenType::SPACE, "", 3, 2),
		Token(TokenType::SPACE, "", 3, 3),
		Token(TokenType::SPACE, "", 3, 4),
		Token(TokenType::ON, "", 3, 5),
		Token(TokenType::SPACE, "", 3, 7),
		Token(TokenType::CUSTOM_NAME, "Play", 3, 8),
		Token(TokenType::SPACE, "", 3, 12),
		Token(TokenType::ARROW, "", 3, 13),
		Token(TokenType::SPACE, "", 3, 15),
		Token(TokenType::CUSTOM_NAME, "RunningState", 3, 16),
		Token(TokenType::NEW_LINE, "", 3, 28),
		Token(TokenType::SPACE, "", 4, 2),							//new
	};

	std::vector<Token> optimized = Optimizer::clearStringEndings(tokens);

	ASSERT_EQ(optimized.size(), 23);


	validateToken(optimized[0], TokenType::NEW_LINE, "", 0, 2);
	validateToken(optimized[1], TokenType::SPACE, "", 1, 2);
	validateToken(optimized[2], TokenType::CUSTOM_NAME, "TestStateMachine", 1, 3);
	validateToken(optimized[3], TokenType::NEW_LINE, "", 1, 19);
	validateToken(optimized[4], TokenType::SPACE, "", 2, 2);
	validateToken(optimized[5], TokenType::SPACE, "", 2, 3);
	validateToken(optimized[6], TokenType::CUSTOM_NAME, "IdleState", 2, 4);
	validateToken(optimized[7], TokenType::SPACE, "", 2, 12);
	validateToken(optimized[8], TokenType::DEFAULT, "", 2, 13);
	validateToken(optimized[9], TokenType::SPACE, "", 2, 20);
	validateToken(optimized[10], TokenType::COMMENT, " some comment  here   ", 2, 21);
	validateToken(optimized[11], TokenType::NEW_LINE, "", 2, 46);
	validateToken(optimized[12], TokenType::SPACE, "", 3, 2);
	validateToken(optimized[13], TokenType::SPACE, "", 3, 3);
	validateToken(optimized[14], TokenType::SPACE, "", 3, 4);
	validateToken(optimized[15], TokenType::ON, "", 3, 5);
	validateToken(optimized[16], TokenType::SPACE, "", 3, 7);
	validateToken(optimized[17], TokenType::CUSTOM_NAME, "Play", 3, 8);
	validateToken(optimized[18], TokenType::SPACE, "", 3, 12);
	validateToken(optimized[19], TokenType::ARROW, "", 3, 13);
	validateToken(optimized[20], TokenType::SPACE, "", 3, 15);
	validateToken(optimized[21], TokenType::CUSTOM_NAME, "RunningState", 3, 16);
	validateToken(optimized[22], TokenType::NEW_LINE, "", 3, 28);
}

TEST(OptimizerTest, clearNewLinesOnStart)
{
	std::vector<Token> tokens = {
		Token(TokenType::NEW_LINE, "", 0, 1),
		Token(TokenType::NEW_LINE, "", 1, 1),
		Token(TokenType::NEW_LINE, "", 2, 1),
		Token(TokenType::SPACE, "", 3, 2),
		Token(TokenType::CUSTOM_NAME, "TestStateMachine", 1, 3),
		Token(TokenType::SPACE, "", 1, 19),
		Token(TokenType::SPACE, "", 2, 2),
		Token(TokenType::NEW_LINE, "", 1, 19),
		Token(TokenType::CUSTOM_NAME, "RunningState", 3, 16),
		Token(TokenType::NEW_LINE, "", 3, 28),
		Token(TokenType::SPACE, "", 4, 2),
	};

	std::vector<Token> optimized = Optimizer::clearNewLinesOnStart(tokens);

	ASSERT_EQ(optimized.size(), 8);

	validateToken(optimized[0], TokenType::SPACE, "", 3, 2);
	validateToken(optimized[1], TokenType::CUSTOM_NAME, "TestStateMachine", 1, 3);
	validateToken(optimized[2], TokenType::SPACE, "", 1, 19);
	validateToken(optimized[3], TokenType::SPACE, "", 2, 2);
	validateToken(optimized[4], TokenType::NEW_LINE, "", 1, 19);
	validateToken(optimized[5], TokenType::CUSTOM_NAME, "RunningState", 3, 16);
	validateToken(optimized[6], TokenType::NEW_LINE, "", 3, 28);
	validateToken(optimized[7], TokenType::SPACE, "", 4, 2);
}

TEST(OptimizerTest, clearNewLinesOnEnd)
{
	std::vector<Token> tokens = {
		Token(TokenType::NEW_LINE, "", 2, 1),
		Token(TokenType::SPACE, "", 3, 2),
		Token(TokenType::CUSTOM_NAME, "TestStateMachine", 1, 3),
		Token(TokenType::SPACE, "", 1, 19),
		Token(TokenType::SPACE, "", 2, 2),
		Token(TokenType::NEW_LINE, "", 1, 19),
		Token(TokenType::NEW_LINE, "", 3, 28),
		Token(TokenType::SPACE, "", 4, 2),
		Token(TokenType::NEW_LINE, "", 5, 1),
		Token(TokenType::NEW_LINE, "", 6, 1),
		Token(TokenType::NEW_LINE, "", 7, 1),

	};

	std::vector<Token> optimized = Optimizer::clearNewLinesOnEnd(tokens);

	ASSERT_EQ(optimized.size(), 8);

	validateToken(optimized[0], TokenType::NEW_LINE, "", 2, 1);
	validateToken(optimized[1], TokenType::SPACE, "", 3, 2);
	validateToken(optimized[2], TokenType::CUSTOM_NAME, "TestStateMachine", 1, 3);
	validateToken(optimized[3], TokenType::SPACE, "", 1, 19);
	validateToken(optimized[4], TokenType::SPACE, "", 2, 2);
	validateToken(optimized[5], TokenType::NEW_LINE, "", 1, 19);
	validateToken(optimized[6], TokenType::NEW_LINE, "", 3, 28);
	validateToken(optimized[7], TokenType::SPACE, "", 4, 2);

}

TEST(OptimizerTest, clearSpacesOnStart)
{
	std::vector<Token> tokens = {
		Token(TokenType::SPACE, "", 1, 0),                      // -
		Token(TokenType::SPACE, "", 1, 2),						// -
		Token(TokenType::CUSTOM_NAME, "TestStateMachine", 1, 3),
		Token(TokenType::NEW_LINE, "", 1, 19),
		Token(TokenType::SPACE, "", 2, 0),                      // -
		Token(TokenType::SPACE, "", 2, 2),                      // -
		Token(TokenType::SPACE, "", 2, 3),
		Token(TokenType::CUSTOM_NAME, "IdleState", 2, 4),
		Token(TokenType::SPACE, "", 2, 12),
		Token(TokenType::DEFAULT, "", 2, 13),
		Token(TokenType::SPACE, "", 2, 20),
		Token(TokenType::COMMENT, " some comment  here   ", 2, 21),
		Token(TokenType::NEW_LINE, "", 2, 46),
		Token(TokenType::SPACE, "", 3, 0),                      // -
		Token(TokenType::SPACE, "", 3, 2),                      // -
		Token(TokenType::SPACE, "", 3, 3),
		Token(TokenType::SPACE, "", 3, 4),
		Token(TokenType::ON, "", 3, 5),
		Token(TokenType::SPACE, "", 3, 7),
		Token(TokenType::CUSTOM_NAME, "Play", 3, 8),
		Token(TokenType::SPACE, "", 3, 12),
		Token(TokenType::ARROW, "", 3, 13),
		Token(TokenType::SPACE, "", 3, 15),
		Token(TokenType::CUSTOM_NAME, "RunningState", 3, 16),
		Token(TokenType::NEW_LINE, "", 3, 28),
	};

	std::vector<Token> optimized = Optimizer::clearSpacesOnStart(tokens);

	ASSERT_EQ(optimized.size(), 19);
	validateToken(optimized[0], TokenType::CUSTOM_NAME, "TestStateMachine", 1, 3);
	validateToken(optimized[1], TokenType::NEW_LINE, "", 1, 19);
	validateToken(optimized[2], TokenType::SPACE, "", 2, 3);
	validateToken(optimized[3], TokenType::CUSTOM_NAME, "IdleState", 2, 4);
	validateToken(optimized[4], TokenType::SPACE, "", 2, 12);
	validateToken(optimized[5], TokenType::DEFAULT, "", 2, 13);
	validateToken(optimized[6], TokenType::SPACE, "", 2, 20);
	validateToken(optimized[7], TokenType::COMMENT, " some comment  here   ", 2, 21);
	validateToken(optimized[8], TokenType::NEW_LINE, "", 2, 46);
	validateToken(optimized[9], TokenType::SPACE, "", 3, 3);
	validateToken(optimized[10], TokenType::SPACE, "", 3, 4);
	validateToken(optimized[11], TokenType::ON, "", 3, 5);
	validateToken(optimized[12], TokenType::SPACE, "", 3, 7);
	validateToken(optimized[13], TokenType::CUSTOM_NAME, "Play", 3, 8);
	validateToken(optimized[14], TokenType::SPACE, "", 3, 12);
	validateToken(optimized[15], TokenType::ARROW, "", 3, 13);
	validateToken(optimized[16], TokenType::SPACE, "", 3, 15);
	validateToken(optimized[17], TokenType::CUSTOM_NAME, "RunningState", 3, 16);
	validateToken(optimized[18], TokenType::NEW_LINE, "", 3, 28);
}

TEST(OptimizerTest, removeSpacesInMiddle)
{
	std::vector<Token> tokens = {
		Token(TokenType::CUSTOM_NAME, "TestStateMachine", 1, 3),
		Token(TokenType::NEW_LINE, "", 1, 19),
		Token(TokenType::SPACE, "", 2, 3),
		Token(TokenType::CUSTOM_NAME, "IdleState", 2, 4),
		Token(TokenType::SPACE, "", 2, 12),
		Token(TokenType::DEFAULT, "", 2, 13),
		Token(TokenType::SPACE, "", 2, 20),
		Token(TokenType::COMMENT, " some comment  here   ", 2, 21),
		Token(TokenType::NEW_LINE, "", 2, 46),
		Token(TokenType::SPACE, "", 3, 3),
		Token(TokenType::SPACE, "", 3, 4),
		Token(TokenType::ON, "", 3, 5),
		Token(TokenType::SPACE, "", 3, 7),
		Token(TokenType::CUSTOM_NAME, "Play", 3, 8),
		Token(TokenType::SPACE, "", 3, 12),
		Token(TokenType::ARROW, "", 3, 13),
		Token(TokenType::SPACE, "", 3, 15),
		Token(TokenType::CUSTOM_NAME, "RunningState", 3, 16),
		Token(TokenType::NEW_LINE, "", 3, 28),
	};

	std::vector<Token> optimized = Optimizer::removeSpacesInMiddle(tokens);

	ASSERT_EQ(optimized.size(), 14);
	validateToken(optimized[0], TokenType::CUSTOM_NAME, "TestStateMachine", 1, 3);
	validateToken(optimized[1], TokenType::NEW_LINE, "", 1, 19);
	validateToken(optimized[2], TokenType::SPACE, "", 2, 3);
	validateToken(optimized[3], TokenType::CUSTOM_NAME, "IdleState", 2, 4);
	validateToken(optimized[4], TokenType::DEFAULT, "", 2, 13);
	validateToken(optimized[5], TokenType::COMMENT, " some comment  here   ", 2, 21);
	validateToken(optimized[6], TokenType::NEW_LINE, "", 2, 46);
	validateToken(optimized[7], TokenType::SPACE, "", 3, 3);
	validateToken(optimized[8], TokenType::SPACE, "", 3, 4);
	validateToken(optimized[9], TokenType::ON, "", 3, 5);
	validateToken(optimized[10], TokenType::CUSTOM_NAME, "Play", 3, 8);
	validateToken(optimized[11], TokenType::ARROW, "", 3, 13);
	validateToken(optimized[12], TokenType::CUSTOM_NAME, "RunningState", 3, 16);
	validateToken(optimized[13], TokenType::NEW_LINE, "", 3, 28);
}