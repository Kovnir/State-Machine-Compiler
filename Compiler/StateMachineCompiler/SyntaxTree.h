#pragma once
#include "TreeNode.cpp"
#include "TokenSequence.cpp"


class SyntaxTree
{
private:
	void parseTokensInner(const std::vector<Token>& tokens);
	const Token* PickWithValidation(TokenSequence& seq, bool allowNull);
	const void CreateRootNode(const Token* val);
	const void ValidateTokenType(const Token* val);
	const void Throw(std::string error, const Token* token);
	std::unique_ptr<StateMachine> root = nullptr;
	TreeNode* current = nullptr;
public:
	SyntaxTree() = default;
	std::string parseTokens(const std::vector<Token>& tokens);
};

