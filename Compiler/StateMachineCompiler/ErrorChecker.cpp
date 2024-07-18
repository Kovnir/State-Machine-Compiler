#include "ErrorChecker.h"
#include <unordered_set>

using namespace std;

//ensure there are no tokens with type 
// SLASH_STAR,
// SPACE,
// STAR,
// COMMENT,
// STAR_SLASH,

const unordered_set<TokenType> prohibitedTokens = {
	TokenType::SLASH_STAR,
	TokenType::SPACE,
	TokenType::STAR,
	TokenType::COMMENT,
	TokenType::STAR_SLASH
};

vector<string> ErrorChecker::checkErrors(const vector<Token>& tokens)
{
	vector<string> errors;
	for (auto const& token : tokens)
	{
		if (prohibitedTokens.find(token.type) != prohibitedTokens.end())
		{
			addError(token, errors);
		}
	}
	return errors;
}

void ErrorChecker::addError(const Token& token, vector<string>& errors)
{
	errors.emplace_back("Unexpected token " + token.toString() + " found. Line: " + std::to_string(token.line) + " Position: " + std::to_string(token.position));
}