#include "pch.h"
#include <gtest/gtest.h>
#include "../../Compiler/StateMachineCompiler/Tokenizer.cpp"
#include "../../Compiler/StateMachineCompiler/Optimizer.cpp"


TEST(TokenizerTest, Parse)
{
    std::string input = R"(/*
 * TestStateMachine
 *  IdleState default // some comment  here   
 *   on Play -> RunningState
 */)";


	std::vector<Token> tokens = Tokenizer::parse(input);
	
	ASSERT_EQ(tokens.size(), 32);

	EXPECT_EQ(tokens[0].type, TokenType::SLASH_STAR);
	EXPECT_EQ(tokens[1].type, TokenType::NEW_LINE);
	EXPECT_EQ(tokens[2].type, TokenType::SPACE);
	EXPECT_EQ(tokens[3].type, TokenType::STAR);
	EXPECT_EQ(tokens[4].type, TokenType::SPACE);
	EXPECT_EQ(tokens[5].type, TokenType::CUSTOM_NAME);
	EXPECT_EQ(tokens[6].type, TokenType::NEW_LINE);
	EXPECT_EQ(tokens[7].type, TokenType::SPACE);
	EXPECT_EQ(tokens[8].type, TokenType::STAR);
	EXPECT_EQ(tokens[9].type, TokenType::SPACE);
	EXPECT_EQ(tokens[10].type, TokenType::SPACE);
	EXPECT_EQ(tokens[11].type, TokenType::CUSTOM_NAME);
	EXPECT_EQ(tokens[12].type, TokenType::SPACE);
	EXPECT_EQ(tokens[13].type, TokenType::DEFAULT);
	EXPECT_EQ(tokens[14].type, TokenType::SPACE);
	EXPECT_EQ(tokens[15].type, TokenType::COMMENT);
	EXPECT_EQ(tokens[16].type, TokenType::NEW_LINE);
	EXPECT_EQ(tokens[17].type, TokenType::SPACE);
	EXPECT_EQ(tokens[18].type, TokenType::STAR);
	EXPECT_EQ(tokens[19].type, TokenType::SPACE);
	EXPECT_EQ(tokens[20].type, TokenType::SPACE);
	EXPECT_EQ(tokens[21].type, TokenType::SPACE);
	EXPECT_EQ(tokens[22].type, TokenType::ON);
	EXPECT_EQ(tokens[23].type, TokenType::SPACE);
	EXPECT_EQ(tokens[24].type, TokenType::CUSTOM_NAME);
	EXPECT_EQ(tokens[25].type, TokenType::SPACE);
	EXPECT_EQ(tokens[26].type, TokenType::ARROW);
	EXPECT_EQ(tokens[27].type, TokenType::SPACE);
	EXPECT_EQ(tokens[28].type, TokenType::CUSTOM_NAME);
	EXPECT_EQ(tokens[29].type, TokenType::NEW_LINE);
	EXPECT_EQ(tokens[30].type, TokenType::SPACE);
	EXPECT_EQ(tokens[31].type, TokenType::STAR_SLASH);
}

TEST(OptimizerTest, clearEverythingBeforeStar)
{
	std::vector<Token> tokens = {
		Token(TokenType::SLASH_STAR, "", 1, 1),						// -
		Token(TokenType::NEW_LINE, "", 1, 3),
		Token(TokenType::SPACE, "", 2, 1),							// -
		Token(TokenType::STAR, "", 2, 2),							// -
		Token(TokenType::SPACE, "", 2, 3),
		Token(TokenType::CUSTOM_NAME, "TestStateMachine", 2, 4),
		Token(TokenType::NEW_LINE, "", 2, 20),
		Token(TokenType::SPACE, "", 3, 1),							// -
		Token(TokenType::STAR, "", 3, 2),							// -
		Token(TokenType::SPACE, "", 3, 3),
		Token(TokenType::SPACE, "", 3, 4),
		Token(TokenType::CUSTOM_NAME, "IdleState", 3, 5),
		Token(TokenType::SPACE, "", 3, 13),
		Token(TokenType::DEFAULT, "", 3, 14),
		Token(TokenType::SPACE, "", 3, 21),
		Token(TokenType::COMMENT, " some comment", 3, 22),
		Token(TokenType::NEW_LINE, "", 3, 37),
		Token(TokenType::SPACE, "", 4, 1),							// -
		Token(TokenType::STAR, "", 4, 2),							// -
		Token(TokenType::SPACE, "", 4, 3),
		Token(TokenType::SPACE, "", 4, 4),
		Token(TokenType::SPACE, "", 4, 5),
		Token(TokenType::ON, "", 4, 6),
		Token(TokenType::SPACE, "", 4, 8),
		Token(TokenType::CUSTOM_NAME, "Play", 4, 9),
		Token(TokenType::SPACE, "", 4, 13),
		Token(TokenType::ARROW, "", 4, 14),
		Token(TokenType::SPACE, "", 4, 16),
		Token(TokenType::CUSTOM_NAME, "RunningState", 4, 17),
		Token(TokenType::NEW_LINE, "", 4, 29),
		Token(TokenType::STAR_SLASH, "", 5, 1)						// -
	};
	std::vector<Token> optimized = Optimizer::clearEverythingBeforeStar(tokens);


	ASSERT_EQ(optimized.size(), 23);

	EXPECT_EQ(optimized[0].type, TokenType::NEW_LINE);
	EXPECT_EQ(optimized[1].type, TokenType::SPACE);
	EXPECT_EQ(optimized[2].type, TokenType::CUSTOM_NAME);
	EXPECT_EQ(optimized[3].type, TokenType::NEW_LINE);
	EXPECT_EQ(optimized[4].type, TokenType::SPACE);
	EXPECT_EQ(optimized[5].type, TokenType::SPACE);
	EXPECT_EQ(optimized[6].type, TokenType::CUSTOM_NAME);
	EXPECT_EQ(optimized[7].type, TokenType::SPACE);
	EXPECT_EQ(optimized[8].type, TokenType::DEFAULT);
	EXPECT_EQ(optimized[9].type, TokenType::SPACE);
	EXPECT_EQ(optimized[10].type, TokenType::COMMENT);
	EXPECT_EQ(optimized[11].type, TokenType::NEW_LINE);
	EXPECT_EQ(optimized[12].type, TokenType::SPACE);
	EXPECT_EQ(optimized[13].type, TokenType::SPACE);
	EXPECT_EQ(optimized[14].type, TokenType::SPACE);
	EXPECT_EQ(optimized[15].type, TokenType::ON);
	EXPECT_EQ(optimized[16].type, TokenType::SPACE);
	EXPECT_EQ(optimized[17].type, TokenType::CUSTOM_NAME);
	EXPECT_EQ(optimized[18].type, TokenType::SPACE);
	EXPECT_EQ(optimized[19].type, TokenType::ARROW);
	EXPECT_EQ(optimized[20].type, TokenType::SPACE);
	EXPECT_EQ(optimized[21].type, TokenType::CUSTOM_NAME);
	EXPECT_EQ(optimized[22].type, TokenType::NEW_LINE);


}
