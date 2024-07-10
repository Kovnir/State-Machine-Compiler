#include "SyntaxTree.h"
#include "Token.cpp"


void SyntaxTree::addToken(Token val)
{
	/*
	SLASH_STAR,   - error
	STAR,		  - error
	STAR_SLASH,   - error
	SIZE_OF_ENUM  - error
	*/

	if (val.type == TokenType::SIZE_OF_ENUM)
	{
		throw std::invalid_argument("This token should not be added to the tree");
	}

	if (val.type == TokenType::SLASH_STAR)
	{
		throw std::invalid_argument("All SLASH_STAR tokens should be already removed on this stage.");
	}

	if (val.type == TokenType::STAR_SLASH)
	{
		throw std::invalid_argument("All STAR_SLASH tokens should be already removed on this stage.");
	}

	if (val.type == TokenType::STAR)
	{
		throw std::invalid_argument("All STAR tokens should be already removed on this stage.");
	}


	if (root == nullptr)
	{
		if (val.type == TokenType::CUSTOM_NAME)
		{
			root = std::make_unique<StateMachine>(val);
			current = root.get();
			return;
		}
		else
		{
			//throw error
			throw std::invalid_argument("Root must be a custom name");
		}
	}

	/*
	Token Types:
	*	SPACE,
	*	CUSTOM_NAME,
	*	DEFAULT,
	*	ON,
	*	ARROW,
	*	NEW_LINE,
	*	COMMENT,
	*/

	/*
	Node Types:
	*	STATE_MACHINE,
	*	STATE,
	*	TRIGGER,
	*	SIZE_OF_ENUM,
		*/
	switch (current->getType())
	{
	case NodeType::STATE_MACHINE:
		return HandleStateMachine(val);
	case NodeType::STATE:
		return HandleState(val);
	case NodeType::TRIGGER:
		return HandleTrigger(val);

	

	default:
		throw std::invalid_argument("Can't Find Type " + root->data.toString());
		break;
	}

}

void SyntaxTree::HandleStateMachine(Token& val)
{
	StateMachine* sm = dynamic_cast<StateMachine*>(current);
	//only NEW_LINE, COMMENT allowed
	switch (val.type)
	{
	case TokenType::NEW_LINE:
		//?????
		return;
	case TokenType::COMMENT:
		sm->comment = val.value;
		return;
	default:
		throw std::invalid_argument("Should be comment or new line after state machine name");
	}
}

void SyntaxTree::HandleState(Token& val)
{
	State* state = dynamic_cast<State*>(current);
	//only NEW_LINE, COMMENT, DEFAULT allowed
	switch (val.type)
	{
	case TokenType::NEW_LINE:
		//?????
		return;
	case TokenType::DEFAULT:
		state->isDefault = true;
		return;
	case TokenType::COMMENT:
		state->comment = val.value;
		return;
	default:
		throw std::invalid_argument("Should be comment, new line or 'default after state name");
	}
}


void SyntaxTree::HandleTrigger(Token& val)
{
	Trigger* state = dynamic_cast<Trigger*>(current);
	//only , COMMENT, DEFAULT allowed
	switch (val.type)
	{
	case TokenType::NEW_LINE:
		//?????
		return;
	case TokenType::DEFAULT:
		//?????
//		state->isDefault = true;
		return;
	case TokenType::COMMENT:
		state->comment = val.value;
		return;
	default:
		throw std::invalid_argument("Should be comment, new line or 'default after state name");
	}
}
