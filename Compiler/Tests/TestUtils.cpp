#pragma once

#include "pch.h"
#include <gtest/gtest.h>
#include "../StateMachineCompiler/Token.cpp"

inline void validateToken(const Token& token, TokenType type, const string& value, int line, int position)
{
	EXPECT_EQ(token.type, type) << "Token type mismatch at line " << line << ", position " << position;
	EXPECT_EQ(token.value, value) << "Token type mismatch at line " << line << ", position " << position;
	EXPECT_EQ(token.line, line) << "Token type mismatch at line " << line << ", position " << position;
	EXPECT_EQ(token.position, position) << "Token type mismatch at line " << line << ", position " << position;
}