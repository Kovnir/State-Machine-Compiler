#include "pch.h"
#include <gtest/gtest.h>
#include "../StateMachineCompiler/ErrorChecker.cpp"


TEST(ErrorChecker, checkErrors)
{
	std::vector<Token> tokens = {
		Token(TokenType::COMMENT, " some comment  here   ", 2, 21), // error
		Token(TokenType::NEW_LINE, "", 2, 46),
		Token(TokenType::SPACE, "", 3, 0), // error
		Token(TokenType::STAR, "", 3, 1), // error
		Token(TokenType::SLASH_STAR, "", 0, 0), // error
		Token(TokenType::NEW_LINE, "", 0, 2),
		Token(TokenType::SPACE, "", 1, 0), // error
		Token(TokenType::STAR, "", 1, 1), // error
		Token(TokenType::SPACE, "", 1, 2), // error
		Token(TokenType::CUSTOM_NAME, "TestStateMachine", 1, 3),
		Token(TokenType::NEW_LINE, "", 1, 19),
		Token(TokenType::CUSTOM_NAME, "IdleState", 2, 4),
		Token(TokenType::DEFAULT, "", 2, 13),
		Token(TokenType::STAR_SLASH, "", 4, 0), // error
		Token(TokenType::SPACE, "", 2, 20), // error
		Token(TokenType::NEW_LINE, "", 2, 46),
	};

	//TokenType::SLASH_STAR,
	//TokenType::SPACE,
	//TokenType::STAR,
	//TokenType::COMMENT,
	//TokenType::STAR_SLASH


	std::vector<std::string> optimized = ErrorChecker::checkErrors(tokens);


	EXPECT_EQ(optimized.size(), 9);

	EXPECT_EQ(optimized[0], "Unexpected token [COMMENT ( some comment  here   )] found. Line: 2 Position: 21");
	EXPECT_EQ(optimized[1], "Unexpected token [SPACE] found. Line: 3 Position: 0");
	EXPECT_EQ(optimized[2], "Unexpected token [STAR] found. Line: 3 Position: 1");
	EXPECT_EQ(optimized[3], "Unexpected token [SLASH_STAR] found. Line: 0 Position: 0");
	EXPECT_EQ(optimized[4], "Unexpected token [SPACE] found. Line: 1 Position: 0");
	EXPECT_EQ(optimized[5], "Unexpected token [STAR] found. Line: 1 Position: 1");
	EXPECT_EQ(optimized[6], "Unexpected token [SPACE] found. Line: 1 Position: 2");
	EXPECT_EQ(optimized[7], "Unexpected token [STAR_SLASH] found. Line: 4 Position: 0");
	EXPECT_EQ(optimized[8], "Unexpected token [SPACE] found. Line: 2 Position: 20");
}