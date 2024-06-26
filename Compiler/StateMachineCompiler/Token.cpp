#pragma once
#include <string>

enum class TokenType
{
	SLASH_STAR,
	SPACE,
	STAR,
	CUSTOM_NAME,
	DEFAULT,
	ON,
	ARROW,
	NEW_LINE,
	COMMENT,
	STAR_SLASH,
	SIZE_OF_ENUM
};


struct Token
{
public:
	TokenType type;
	std::string value;
	int line;
	int position;

	Token(TokenType type, const std::string& value, int line, int position) : type(type), value(value), line(line), position(position) {}

	std::string toString() const
	{
		std::string result = "[";
		result += TokenTypeNames[(int) type];
		if (type == TokenType::CUSTOM_NAME || type == TokenType::COMMENT)
		{
			result += " (" + value + ")";
		}
		result += "]";
		return result;
	}

private:
	const char* TokenTypeNames[10] = {
		"SLASH_STAR",
		"SPACE",
		"STAR",
		"CUSTOM_NAME",
		"DEFAULT",
		"ON",
		"ARROW",
		"NEW_LINE",
		"COMMENT",
		"STAR_SLASH",
	};

	// statically check that the size of array fits the number of enum values
	static_assert(sizeof(Token::TokenTypeNames) / sizeof(char*) == (int) TokenType::SIZE_OF_ENUM
		, "sizes dont match");
};

