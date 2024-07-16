# State-Machine-Compiler

## Overview

**State-Machine-Compiler** is a C++ project designed to generate type-safe asynchronous state machine code from a high-level abstract language description. This tool allows users to define state machines in a simple, human-readable format and then compiles that description into robust C# code.

### Input Example
The state machine is described using a simple syntax:
```plaintext
/*
 * TestStateMachine
 *   IdleState default
 *     on Play -> RunningState
 *   RunningState
 *     on Pause -> PausedState
 *     on Finish -> FinishedState
 *     on Stop -> IdleState
 *   PausedState
 *     on Resume -> RunningState
 *     on Stop -> IdleState
 *   FinishedState
 *     on Replay -> RunningState
 */
```

- **TestStateMachine**: The name of the state machine.
- **IdleState**: A state (marked as default). Only one state can be marked as default.
- **on Play -> RunningState**: A transition from Idle to Chase on the "PlayerInRadius" event.

## Usage

Compile the project and run the `smc` executable with the path to the state machine definition file. Optionally, use the `--debug` flag for detailed logs.

```sh
smc --debug path/to/state_machine.txt
```

### Example
Here’s how you can define the state machine, compile it, and then integrate the generated C# code.

1. **Define the State Machine** in a text file (e.g., `MonsterStateMachine.cs`):
    ```plaintext
    /*
     * MonsterStateMachine
     *   Idle default
     *      on PlayerInRadius -> Chase
     *   Chase
     *      on PlayerOutOfRange -> Idle
     *      on PlayerInAttackRange -> Attack
     *  Attack
     *      on AttackComplete -> Chase
     *      on PlayerDead -> Idle
     */
    ```

2. **Compile the State Machine** using the `smc` executable:
    ```sh
    smc path/to/MonsterStateMachine.cs
    ```

3. **Generated C# Code**:
    ```csharp
    public sealed class MonsterStateMachine
    {
        public enum SMStatus
        {
            Idle,
            Running,
            Transitioning,
            Failed,
        }

        public abstract class BaseState
        {
            private readonly Func<Task> onEnter;
            private readonly Func<Task> onExit;
            protected MonsterStateMachine monsterStateMachine;

            protected BaseState(Func<Task> onEnter = null, Func<Task> onExit = null)
            {
                this.onEnter = onEnter;
                this.onExit = onExit;
            }

            public void Initialize(MonsterStateMachine monsterStateMachine)
                => this.monsterStateMachine = monsterStateMachine;

            public Task OnEnter() => onEnter?.Invoke() ?? Task.CompletedTask;
            public Task OnExit() => onExit?.Invoke() ?? Task.CompletedTask;
        }

        public interface IIdle
        {
            Task<IChase> PlayerInRadius();
        }

        public interface IChase
        {
            Task<IIdle> PlayerOutOfRange();
            Task<IAttack> PlayerInAttackRange();
        }

        public interface IAttack
        {
            Task<IChase> AttackComplete();
            Task<IIdle> PlayerDead();
        }

        public sealed class Idle : BaseState, IIdle
        {
            public Idle(Func<Task> onEnter = null, Func<Task> onExit = null) : base(onEnter, onExit)
            {
            }

            public Task<IChase> PlayerInRadius() => monsterStateMachine.OnPlayerInRadiusFromIdle();
        }

        public class Chase : BaseState, IChase
        {
            public Chase(Func<Task> onEnter = null, Func<Task> onExit = null) : base(onEnter, onExit)
            {
            }

            public Task<IIdle> PlayerOutOfRange() => monsterStateMachine.OnPlayerOutOfRangeFromChase();
            public Task<IAttack> PlayerInAttackRange() => monsterStateMachine.OnPlayerInAttackRangeFromChase();
        }

        public class Attack : BaseState, IAttack
        {
            public Attack(Func<Task> onEnter = null, Func<Task> onExit = null) : base(onEnter, onExit)
            {
            }

            public Task<IChase> AttackComplete() => monsterStateMachine.OnAttackCompleteFromAttack();
            public Task<IIdle> PlayerDead() => monsterStateMachine.OnPlayerDeadFromAttack();
        }

        private BaseState currentState;

        private Idle idleState;
        private Chase chaseState;
        private Attack attackState;
        public SMStatus Status;

        public MonsterStateMachine(Idle idleState, Chase chaseState, Attack attackState)
        {
            Status = SMStatus.Idle;

            try
            {
                this.idleState = idleState ?? throw new ArgumentNullException(nameof(idleState));
                this.chaseState = chaseState ?? throw new ArgumentNullException(nameof(chaseState));
                this.attackState = attackState ?? throw new ArgumentNullException(nameof(attackState));
            }
            catch (Exception)
            {
                Status = SMStatus.Failed;
                throw;
            }

            idleState.Initialize(this);
            chaseState.Initialize(this);
            attackState.Initialize(this);
        }

        public async Task<IIdle> Run()
        {
            CheckStatus(true);
            await Transit(null, idleState);
            return idleState;
        }

        private async Task<IChase> OnPlayerInRadiusFromIdle()
        {
            CheckStatus();
            await Transit(idleState, chaseState);
            return chaseState;
        }

        private async Task<IIdle> OnPlayerOutOfRangeFromChase()
        {
            CheckStatus();
            await Transit(chaseState, idleState);
            return idleState;
        }

        private async Task<IAttack> OnPlayerInAttackRangeFromChase()
        {
            CheckStatus();
            await Transit(chaseState, attackState);
            return attackState;
        }

        private async Task<IChase> OnAttackCompleteFromAttack()
        {
            CheckStatus();
            await Transit(attackState, chaseState);
            return chaseState;
        }

        private async Task<IIdle> OnPlayerDeadFromAttack()
        {
            CheckStatus();
            await Transit(attackState, idleState);
            return idleState;
        }

        private void CheckStatus(bool firstRun = false)
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
                    throw new InvalidOperationException("Invalid state transition. Current state is not the expected state.");
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
    }
    ```

4. **Use the Generated Constructor** to define the logic:
    ```csharp
    MonsterStateMachine monsterStateMachine = new MonsterStateMachine(
        new MonsterStateMachine.Attack(() =>
        {
            Console.WriteLine("Attack started");
            return Task.CompletedTask;
        }, ()
            =>
        {
            Console.WriteLine("Attack ended");
            return Task.CompletedTask;
        }),
        new MonsterStateMachine.Chase(() =>
        {
            Console.WriteLine("Chase started");
            return Task.CompletedTask;
        }, ()
            =>
        {
            Console.WriteLine("Chase ended");
            return Task.CompletedTask;
        }),
        new MonsterStateMachine.Idle(() =>
        {
            Console.WriteLine("Idle started");
            return Task.CompletedTask;
        }, ()
            =>
        {
            Console.WriteLine("Idle ended");
            return Task.CompletedTask;
        }));
    ```
5. **Use the Generated Code** to run the state machine and switch between states:
    ```csharp
    //transitions
    MonsterStateMachine.IIdle idleState = await monsterStateMachine.Run();
    MonsterStateMachine.IChase chaseState = await idleState.PlayerInRadius();

    MonsterStateMachine.IAttack attackState = await chaseState.PlayerInAttackRange();
    // or
    // idleState = await chaseState.AttackComplete();

    chaseState = await attackState.AttackComplete();
    // or
    // idleState = await attackState.PlayerDead();
    ```

## Debugging

Enable debugging mode with the `--debug` flag to get detailed logs about the state machine compilation process.

## How It Works

1. **Lexical Analysis**: The input string is tokenized into a sequence of tokens. Each token has a type and position (for clear compilation errors) and could have optional data.
    ```cpp
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
        STAR_SLASH
    };
    ```

2. **Token Optimization**: Unnecessary tokens (like comments and spaces) are removed, and the token stream is optimized for parsing.

3. **Syntax Checking**: The token stream is checked for basic syntax errors. Some compilation errors could be generated here.

4. **Syntax Tree Building**: A syntax tree is built from the token stream. If errors are found, they are thrown.

5. **Code Generation**: The validated syntax tree is used to generate C# code for the state machine.

## Flow Diagram

![Flow Diagram](Docs/diagram.png)

## Future Updates
- **Add Comment Transfer**: Implement functionality to transfer comments from the state machine description to the generated code, preserving the context.
- **Enhance UX for Compilation Error Output**: Improve the user experience by providing more detailed and user-friendly error messages during the compilation process, including information for easier debugging.
- **Improve Test Coverage**: Increase the coverage of unit and integration tests to ensure the reliability and robustness of the compiler, catching more edge cases and potential issues.

## Contribution

Feel free to open issues or submit pull requests. Contributions are welcome!

## License

This project is licensed under the MIT License. See the LICENSE file for details.

---

Harness the power of type-safe state machine generation with State-Machine-Compiler! 🚀
