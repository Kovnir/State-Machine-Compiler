#include "pch.h"
#include <gtest/gtest.h>
#include "../../Compiler/StateMachineCompiler/Tokenizer.cpp"


TEST(TokenizerTest, Parse)
{
    std::string input = R"(/*
 * TestStateMachine
 *  IdleState default // some comment  here   
 *   on Play -> RunningState
 */)";


	std::vector<Token> tokens = Tokenizer::Parse(input);
	
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