#include "Token.cpp"
#include <iostream>
#include <memory>
#include <vector>

//std::string input = R"(  /*
//    * TestStateMachine
//    *   IdleState default
//    *     on Play -> RunningState    
//    *   RunningState // some comment
//    *     on Pause -> PausedState    
//    */");
enum class NodeType
{
	STATE_MACHINE,
	STATE,
	TRIGGER,
	SIZE_OF_ENUM,
};

struct TreeNode;
struct StateMachine;
struct State;
struct Trigger;

struct TreeNode {
	Token data;
	std::string comment;

	explicit TreeNode(Token const& val) : data(val) {}
	virtual NodeType getType() const = 0; // виртуальный метод
};

struct StateMachine : public TreeNode {
	using TreeNode::TreeNode;
	std::vector<std::unique_ptr<State>> states;
	/*TreeNode* addChild(Token val) {
		auto child = std::make_unique<TreeNode>(val);
		TreeNode* childPtr = child.get();
		children.push_back(std::move(child));
		return childPtr;
	}*/
	NodeType getType() const override { return NodeType::STATE_MACHINE; }
};

struct State : public TreeNode {

	using TreeNode::TreeNode;
	bool isDefault = false;
	NodeType getType() const override { return NodeType::STATE; }
};

struct Trigger : public TreeNode {
	using TreeNode::TreeNode;
	std::string condition;
	std::string targetStateSting;
	NodeType getType() const override { return NodeType::TRIGGER; }
};