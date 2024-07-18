#pragma once
#include "SyntaxTree.h"
#include "Token.cpp"
#include "TokenSequence.cpp"

using namespace std;

// from this code
//   * TestStateMachine\n\
 //   *   IdleState default\n\
 //   *     on Play -> RunningState\n\
 //   *   RunningState // some comment\n\
 //   *     on Pause -> PausedState    \n\
 //   *     on Finish -> FinishedState \n\
 //   *     on Stop ->    IdleState\n\
 //   *   PausedState\n\
 //   *     on Resume -> RunningState\n\
 //   *     on Stop -> IdleState\n\
 //   *   FinishedState\n\
 //   *     on Replay -> RunningState\n\
 // 
 //we have new tokens:
 // TestStateMachine-NewLine-                       //State Machine Definition
 // IdleState-default-NewLine-                      //State Definition
 // on-Play-Arrow-RunningState-NewLine-             //Transition Definition
 // RunningState-NewLine-							//State Definition
 // on-Pause-Arrow-PausedState-NewLine-				//Transition Definition
 // on-Finish-Arrow-FinishedState-NewLine-			//Transition Definition
 // on-Stop-Arrow-IdleState-NewLine-				//Transition Definition
 // PausedState-NewLine-                            //State Definition
 // on-Resume-Arrow-RunningState-NewLine-			//Transition Definition
 // on-Stop-Arrow-IdleState-NewLine-				//Transition Definition
 // FinishedState-NewLine-                          //State Definition
 // on-Replay-Arrow-RunningState-NewLine			//Transition Definition
//

string SyntaxTree::parseTokens(const vector<Token>& tokens)
{
	try {
		parseTokensInner(tokens);
	}
	catch (const exception& e)
	{
		return e.what();
	}
	catch (...)
	{
		return "Unexpected error";
	}
	return "";
}

void SyntaxTree::parseTokensInner(const vector<Token>& tokens)
{
	TokenSequence sequence(tokens);
	while (sequence.hasNext())
	{
		const Token* token = PickWithValidation(sequence, true);

		if (token == nullptr)
		{
			return;
		}

		if (root == nullptr)
		{
			CreateRootNode(token);
			continue;
		}

		/*
		Possible Token Types:
		*	SPACE,
		*	CUSTOM_NAME,
		*	DEFAULT,
		*	ON,
		*	ARROW,
		*	NEW_LINE,
		*/

		/*
		Possible Node Types:
		*	STATE_MACHINE,
		*	STATE,
		*	TRIGGER,
		*/


		switch (current->getType())
		{
		case NodeType::STATE_MACHINE:
		{
			auto* sm = dynamic_cast<StateMachine*>(current);
			sequence.moveNext();
			token = PickWithValidation(sequence, false);

			//only NEW_LINE allowed
			if (token->type == TokenType::NEW_LINE)
			{
				//after it should always be a state
				sequence.moveNext();
				token = PickWithValidation(sequence, true);

				if (token == nullptr)
				{
					return; //end of document
				}

				if (token->type == TokenType::CUSTOM_NAME)
				{
					current = sm->addState(token);
					sequence.moveNext();
				}
				else
				{
					Throw("After state machine name should be a state name.", token);
				}
			}
			else
			{
				Throw("Should be new line after state machine name.", token);
			}
			break;
		}
		case NodeType::STATE:
		{
			auto state = dynamic_cast<State*>(current);
			//only NEW_LINE, DEFAULT allowed
			switch (token->type)
			{
			case TokenType::NEW_LINE:
				sequence.moveNext();
				// only trigger or next state allowed
				token = PickWithValidation(sequence, true);
				if (token == nullptr)
				{
					return;
				}

				if (token->type == TokenType::CUSTOM_NAME)
				{
					current = root->addState(token);
					sequence.moveNext();
				}
				else if (token->type == TokenType::ON)
				{
					current = state->addTrigger(*token);
					sequence.moveNext();
				}
				else
				{
					Throw("After default should be a new line.", token);
				}
				break;
			case TokenType::DEFAULT:
				state->isDefault = true;
				sequence.moveNext();
				//after it only NEW_LINE allowed
				token = PickWithValidation(sequence, true);
				if (token == nullptr)
				{
					return;
				}

				if (token->type == TokenType::NEW_LINE)
				{
					sequence.moveNext();
					// only trigger or next state allowed
					token = PickWithValidation(sequence, true);
					if (token == nullptr)
					{
						return;
					}

					if (token->type == TokenType::CUSTOM_NAME)
					{
						current = root->addState(token);
						sequence.moveNext();
					}
					else if (token->type == TokenType::ON)
					{
						current = state->addTrigger(*token);
						sequence.moveNext();
					}
					else
					{
						Throw("Unexpected token.", token);
					}
				}
				else
				{
					Throw("After default should be a new line.", token);
				}
				break;
			default:
				Throw("Should be comment, new line or 'default after state name.", token);
			}
			break;
		}
		case NodeType::TRIGGER:
		{
			auto* trigger = dynamic_cast<Trigger*>(current);
			//format always on-TriggerName-Arrow-TargetState-NewLine
			if (token->type != TokenType::CUSTOM_NAME)
			{
				Throw("Trigger format always shouldbe 'on TriggerName -> TargetState'.", token);
			}

			trigger->condition = token->value;
			sequence.moveNext();
			token = PickWithValidation(sequence, false);
			if (token->type != TokenType::ARROW)
			{
				Throw("Trigger format always shouldbe 'on TriggerName -> TargetState'.", token);
			}
			sequence.moveNext();
			token = PickWithValidation(sequence, false);

			if (token->type != TokenType::CUSTOM_NAME)
			{
				Throw("Trigger format always shouldbe 'on TriggerName -> TargetState'.", token);
			}

			trigger->targetStateString = token->value;
			sequence.moveNext();
			token = PickWithValidation(sequence, true);

			if (token == nullptr)
			{
				return;
			}

			if (token->type != TokenType::NEW_LINE)
			{
				Throw("Trigger format always shouldbe 'on TriggerName -> TargetState'.", token);
			}

			//next - trigger, state or code finish
			sequence.moveNext();
			token = PickWithValidation(sequence, true);
			if (token == nullptr)
			{
				return;
			}

			if (token->type == TokenType::CUSTOM_NAME)
			{
				current = root->addState(token);
				sequence.moveNext();
			}
			else if (token->type == TokenType::ON)
			{
				current = trigger->rootState->addTrigger(*token);
				sequence.moveNext();
			}
			else
			{
				Throw("Unexpected token.", token);
			}

			break;
		}

		default:
			Throw("Can't Find Type " + root->data.toString(), token);
			break;
		}
	}
}

const void SyntaxTree::CreateRootNode(const Token* val)
{
	if (val->type == TokenType::CUSTOM_NAME)
	{
		root = make_unique<StateMachine>(*val);
		current = root.get();
		return;
	}
	else
	{
		Throw("Root must be a custom name.", val);
	}
}

const void SyntaxTree::ValidateTokenType(const Token* val)
{
	/*
	SLASH_STAR,   - error
	STAR,		  - error
	STAR_SLASH,   - error
	SIZE_OF_ENUM  - error
	COMMENT		  - error
	*/

	if (val->type == TokenType::SIZE_OF_ENUM)
	{
		Throw("This token should not be added to the tree.", val);
	}

	if (val->type == TokenType::SLASH_STAR)
	{
		Throw("All SLASH_STAR tokens should be already removed on this stage.", val);
	}

	if (val->type == TokenType::STAR_SLASH)
	{
		Throw("All STAR_SLASH tokens should be already removed on this stage.", val);
	}

	if (val->type == TokenType::STAR)
	{
		Throw("All STAR tokens should be already removed on this stage.", val);
	}

	if (val->type == TokenType::COMMENT)
	{
		Throw("All COMMENT tokens should be already removed on this stage.", val);
	}
}

const Token* SyntaxTree::PickWithValidation(TokenSequence& seq, bool allowNull)
{
	const Token* token = seq.peek();
	if (token != nullptr)
	{
		ValidateTokenType(token);
	}
	else
	{
		if (!allowNull)
		{
			throw invalid_argument("Unexpected file ending.");
		}
		return nullptr;
	}
}

const void SyntaxTree::Throw(string error, const Token* token)
{
	string errorText = error + " - Line: " + to_string(token->line) + " Position : " + to_string(token->position);
	throw invalid_argument(errorText);
}
