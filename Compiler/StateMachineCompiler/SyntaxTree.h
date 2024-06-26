#pragma once
#include "TreeNode.cpp"


class SyntaxTree
{
public:
	SyntaxTree() = default;
	void addToken(Token val);
	void HandleStateMachine(Token& val);
	void HandleState(Token& val);
	void HandleTrigger(Token& val);
	std::unique_ptr<TreeNode> root = nullptr;
	TreeNode* current = nullptr;
};

