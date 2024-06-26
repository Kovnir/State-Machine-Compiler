#include "pch.h"
#include <gtest/gtest.h>
#include "../../Compiler/StateMachineCompiler/Tokenizer.cpp"
#include "TestUtils.cpp"


TEST(TokenizerTest, Parse)
{
	std::string input = R"(/*
 * TestStateMachine
 *  IdleState default // some comment  here   
 *   on Play -> RunningState
 */)";


	std::vector<Token> tokens = Tokenizer::parse(input);

	ASSERT_EQ(tokens.size(), 32);

	validateToken(tokens[0], TokenType::SLASH_STAR, "", 0, 0);
	validateToken(tokens[1], TokenType::NEW_LINE, "", 0, 2);
	validateToken(tokens[2], TokenType::SPACE, "", 1, 0);
	validateToken(tokens[3], TokenType::STAR, "", 1, 1);
	validateToken(tokens[4], TokenType::SPACE, "", 1, 2);
	validateToken(tokens[5], TokenType::CUSTOM_NAME, "TestStateMachine", 1, 3);
	validateToken(tokens[6], TokenType::NEW_LINE, "", 1, 19);
	validateToken(tokens[7], TokenType::SPACE, "", 2, 0);
	validateToken(tokens[8], TokenType::STAR, "", 2, 1);
	validateToken(tokens[9], TokenType::SPACE, "", 2, 2);
	validateToken(tokens[10], TokenType::SPACE, "", 2, 3);
	validateToken(tokens[11], TokenType::CUSTOM_NAME, "IdleState", 2, 4);
	validateToken(tokens[12], TokenType::SPACE, "", 2, 13);
	validateToken(tokens[13], TokenType::DEFAULT, "", 2, 14);
	validateToken(tokens[14], TokenType::SPACE, "", 2, 21);
	validateToken(tokens[15], TokenType::COMMENT, " some comment  here   ", 2, 22);
	validateToken(tokens[16], TokenType::NEW_LINE, "", 2, 46);
	validateToken(tokens[17], TokenType::SPACE, "", 3, 0);
	validateToken(tokens[18], TokenType::STAR, "", 3, 1);
	validateToken(tokens[19], TokenType::SPACE, "", 3, 2);
	validateToken(tokens[20], TokenType::SPACE, "", 3, 3);
	validateToken(tokens[21], TokenType::SPACE, "", 3, 4);
	validateToken(tokens[22], TokenType::ON, "", 3, 5);
	validateToken(tokens[23], TokenType::SPACE, "", 3, 7);
	validateToken(tokens[24], TokenType::CUSTOM_NAME, "Play", 3, 8);
	validateToken(tokens[25], TokenType::SPACE, "", 3, 12);
	validateToken(tokens[26], TokenType::ARROW, "", 3, 13);
	validateToken(tokens[27], TokenType::SPACE, "", 3, 15);
	validateToken(tokens[28], TokenType::CUSTOM_NAME, "RunningState", 3, 16);
	validateToken(tokens[29], TokenType::NEW_LINE, "", 3, 28);
	validateToken(tokens[30], TokenType::SPACE, "", 4, 0);
	validateToken(tokens[31], TokenType::STAR_SLASH, "", 4, 1);
}