#pragma once
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
	TRIGGER
};

struct TreeNode;
struct StateMachine;
struct State;
struct Trigger;

struct TreeNode {
	Token data;
	//	std::string comment;

	explicit TreeNode(const Token val) : data(val) {}
	virtual NodeType getType() const = 0;
};

struct StateMachine : public TreeNode {
	using TreeNode::TreeNode;
	std::vector<std::unique_ptr<State>> states;
	State* addState(const Token* val) {
		auto child = std::make_unique<State>(*val);
		State* childPtr = child.get();
		states.push_back(std::move(child));
		return childPtr;
	}
	NodeType getType() const override { return NodeType::STATE_MACHINE; }
};

struct Trigger : public TreeNode {
	using TreeNode::TreeNode;
	State* rootState = nullptr;
	std::string condition;
	std::string targetStateString;
	NodeType getType() const override { return NodeType::TRIGGER; }
};

struct State : public TreeNode {
	using TreeNode::TreeNode;
	bool isDefault = false;
	std::vector<std::unique_ptr<Trigger>> triggers;
	Trigger* addTrigger(const Token& val) {
		auto trigger = std::make_unique<Trigger>(val);
		trigger->rootState = this;
		Trigger* childPtr = trigger.get();
		triggers.push_back(std::move(trigger));
		return childPtr;
	}
	NodeType getType() const override { return NodeType::STATE; }
};

