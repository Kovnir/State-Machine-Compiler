#pragma once
#include "TreeNode.cpp"
#include "TokenSequence.cpp"


class SyntaxTree
{
private:
	void parseTokensInner(const vector<Token>& tokens);
	const Token* PickWithValidation(TokenSequence& seq, bool allowNull);
	const void CreateRootNode(const Token* val);
	const void ValidateTokenType(const Token* val);
	const void Throw(string error, const Token* token);
	TreeNode* current = nullptr;
public:
	SyntaxTree() = default;
	string parseTokens(const vector<Token>& tokens);
	unique_ptr<StateMachine> root = nullptr;
};

