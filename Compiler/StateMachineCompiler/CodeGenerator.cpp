#include "CodeGenerator.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>

std::string CodeGenerator::generate(const StateMachine& stateMachine)
{
	std::string stateMachineName = stateMachine.data.value;
	if (stateMachineName.empty())
	{
		throw std::runtime_error("State machine name is empty");
	}

    if (stateMachineName.find(' ') != std::string::npos)
    {
		throw std::runtime_error("State machine name contains spaces");
    }

	if (stateMachine.states.size() == 0)
	{
		throw std::runtime_error("State machine has no states");
	}

	if (stateMachine.states.size() == 1)
	{
		throw std::runtime_error("State machine has only one state");
	}

    std::string defaultStateName;
	std::vector<std::string> stateNames;

	if (stateMachine.states.size() > 1)
	{
		int defaultStatesCount = 0;
		//state names
		for (const auto& state : stateMachine.states)
		{
			if (state->isDefault)
			{
				defaultStateName = state->data.value;
				defaultStatesCount++;
			}
			std::string stateName = state->data.value;

			if (stateName.empty())
			{
				throw std::runtime_error("State name is empty");
			}
            
			if (stateName.find(' ') != std::string::npos)
			{
				throw std::runtime_error("State name contains spaces");
			}

			stateNames.push_back(stateName);
		}

		if (defaultStatesCount == 0)
		{
			throw std::runtime_error("State machine has no default state");
		}

		if (defaultStatesCount > 1)
		{
			throw std::runtime_error("State machine has more than one default state");
		}

		//check for duplicate state names
		std::sort(stateNames.begin(), stateNames.end());
		auto it = std::unique(stateNames.begin(), stateNames.end());
		if (it != stateNames.end())
		{
			throw std::runtime_error("State machine has duplicate state names");
		}
	}

	std::vector<std::string> source;
	std::vector<std::string> triggersNames;
	std::vector<std::string> triggersDistinations;

	for (const auto& state : stateMachine.states)
	{
		//state name
		std::string stateName = state->data.value;

		//triggers
		for (const auto& trigger : state->triggers)
		{
			//trigger condition
			std::string triggerCondition = trigger->condition;
			if (triggerCondition.empty())
			{
				throw std::runtime_error("Trigger condition is empty");
			}

			if (triggerCondition.find(' ') != std::string::npos)
			{
				throw std::runtime_error("Trigger condition contains spaces");
			}

			//trigger destination
			std::string triggerDestination = trigger->targetStateString;
			if (triggerDestination.empty())
			{
				throw std::runtime_error("Trigger destination is empty");
			}

			if (triggerDestination.find(' ') != std::string::npos)
			{
				throw std::runtime_error("Trigger destination contains spaces");
			}

			source.push_back(stateName);
			triggersNames.push_back(triggerCondition);
			triggersDistinations.push_back(triggerDestination);
		}
	}

    return generateCSharpCode(stateMachineName, defaultStateName, stateNames, source, triggersNames, triggersDistinations);
}


std::string CodeGenerator::generateCSharpCode(
    const std::string& stateMachineName,
    const std::string& defaultStateName,
    const std::vector<std::string>& allStates,
    const std::vector<std::string>& source,
    const std::vector<std::string>& triggersNames,
    const std::vector<std::string>& triggersDistinations)
{
    std::ostringstream code;
    // add imports

    code << R"(
using System;
using System.Threading.Tasks;
)";
	code << "\n";


	// the beginning of the class and the enumeration of states
    code << "public sealed class " << stateMachineName << "\n{\n";
    code << "    public enum SMStatus\n    {\n";
    code << "        Idle,\n";
    code << "        Running,\n";
    code << "        Transitioning,\n";
    code << "        Failed,\n";
    code << "    }\n\n";

    /*
    public abstract class BaseState
    {
        private readonly Func<Task> onEnter;
        private readonly Func<Task> onExit;
        protected TestStateMachine testStateMachine;

        protected BaseState(Func<Task> onEnter = null, Func<Task> onExit = null)
        {
            this.onEnter = onEnter;
            this.onExit = onExit;
        }

        public void Initialize(TestStateMachine testStateMachine)
            = > this.testStateMachine = testStateMachine;

        public Task OnEnter() = > onEnter ? .Invoke() ? ? Task.CompletedTask;
        public Task OnExit() = > onExit ? .Invoke() ? ? Task.CompletedTask;
    }*/
	// base class for states
    code << R"(    public abstract class BaseState
    {
        private readonly Func<Task> onEnter;
        private readonly Func<Task> onExit;
        protected )" << stateMachineName << R"( stateMachine;

        protected BaseState(Func<Task> onEnter = null, Func<Task> onExit = null)
        {
            this.onEnter = onEnter;
            this.onExit = onExit;
        }

        public void Initialize()" << stateMachineName << R"( stateMachine) => this.stateMachine = stateMachine;
        public Task OnEnter() => onEnter?.Invoke() ?? Task.CompletedTask;
        public Task OnExit() => onExit?.Invoke() ?? Task.CompletedTask;
    })";
    code << "\n\n";

    /*
    public interface IRunningState
    {
        Task<IPausedState> Pause();
        Task<IFinishedState> Finish();
        Task<IIdleState> Stop();
    }
*/
	// Generate state interfaces
    for (const auto& state : allStates) {
        code << "    public interface I" << state << "\n    {\n";
        for (size_t i = 0; i < source.size(); ++i) {
            if (source[i] == state) {
                code << "        Task<I" << triggersDistinations[i] << "> " << triggersNames[i] << "();\n";
            }
        }
        code << "    }\n\n";
    }

    /*
    public class RunningState : BaseState, IRunningState
    {
        public RunningState(Func<Task> onEnter = null, Func<Task> onExit = null) : base(onEnter, onExit)
        {
        }

        public Task<IPausedState> Pause() = > testStateMachine.OnPauseFromRunning();

        public Task<IIdleState> Stop() = > testStateMachine.OnStopFromRunning();

        public Task<IFinishedState> Finish() = > testStateMachine.OnFinishFromRunning();
    }
*/
	// Generate state classes
    for (const auto& state : allStates) {
        code << "    public sealed class " << state << " : BaseState, I" << state << "\n    {\n";
        code << "        public " << state << "(Func<Task> onEnter = null, Func<Task> onExit = null) : base(onEnter, onExit)\n        {\n";
        code << "        }\n\n";
        for (size_t i = 0; i < source.size(); ++i) {
            if (source[i] == state) {
                code << "        public Task<I" << triggersDistinations[i] << "> " << triggersNames[i] << "() => stateMachine.On" << triggersNames[i] << "From" << state << "();\n";
            }
        }
        code << "    }\n\n";
    }

    /*
    private BaseState currentState;

    private IdleState idleState;
    private RunningState runningState;
    private PausedState pausedState;
    private FinishedState finishedState;
    public SMStatus Status;
*/
	// Final parts of the class
    code << R"(    private BaseState currentState;

)";

    for (const auto& state : allStates) {
        code << "    private " << state << " " << state << "State;\n";
    }
    code << "    public SMStatus Status { get; private set;}\n\n";


    code << "    public " << stateMachineName << "(";
    for (const auto& state : allStates) {
        code << state << " " << state << "State, ";
    }
    code.seekp(-2, std::ios_base::end); // Remove the trailing comma and space
    code << ")\n    {\n";
    code << "        Status = SMStatus.Idle;\n";
    code << "        try\n        {\n";
    for (const auto& state : allStates) {
        code << "            this." << state << "State = " << state << "State ?? throw new ArgumentNullException(nameof(" << state << "State));\n";
    }
    code << "        }\n        catch (Exception)\n        {\n";
    code << "            Status = SMStatus.Failed;\n";
    code << "            throw;\n";
    code << "        }\n\n";
    for (const auto& state : allStates) {
        code << "        " << state << "State.Initialize(this);\n";
    }
    code << "    }\n\n";



    // State Machine Start
    /*
    public async Task<IIdleState> Run()
    {
        CheckStatus(true);

        await Transit(null, idleState);
        return idleState;
    }
    */
    code << R"(
    public async Task<)" << defaultStateName << R"(> Run()
    {
        CheckStatus(true);

        await Transit(null, )" << defaultStateName << R"();
        return )" << defaultStateName << R"(;
    })";
    code << "\n\n";



	// Transition methods
    for (size_t i = 0; i < source.size(); ++i) {
        code << "    private async Task<I" << triggersDistinations[i] << "> On" << triggersNames[i] << "From" << source[i] << "()\n    {\n";
        code << "        CheckStatus();\n";
        code << "        await Transit(" << source[i] << "State, " << triggersDistinations[i] << "State);\n";
        code << "        return " << triggersDistinations[i] << "State;\n";
        code << "    }\n\n";
    }

    code << R"(    private void CheckStatus(bool firstRun = false)
    {
        switch (Status)
        {
        case SMStatus.Failed:
            throw new InvalidOperationException("StateMachine failed");
        case SMStatus.Running:
            if (firstRun)
            {
                Status = SMStatus.Failed;
                throw new InvalidOperationException("StateMachine is already running");
            }
            break;
        case SMStatus.Transitioning:
            Status = SMStatus.Failed;
            throw new InvalidOperationException("StateMachine is transitioning");
        }
    }

    private async Task Transit(BaseState from, BaseState to)
    {
        try
        {
            Status = SMStatus.Transitioning;
            if (from != null && from != currentState)
            {
                throw new InvalidOperationException(
                    "Invalid state transition. Current state is not the expected state.");
            }

            if (from != null)
            {
                await from.OnExit();
            }

            currentState = to;
            await to.OnEnter();
            Status = SMStatus.Running;
        }
        catch (Exception)
        {
            Status = SMStatus.Failed;
            throw;
        }
    }
})";

    return code.str();
}