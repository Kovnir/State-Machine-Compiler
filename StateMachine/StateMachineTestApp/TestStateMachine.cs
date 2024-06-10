namespace StateMachineTestApp;

/*
 * TestStateMachine
 *   IdleState default
 *     On Play -> RunningState
 *   RunningState
 *     On Pause -> PausedState
 *     On Finish -> FinishedState
 *     On Stop -> IdleState
 *   PausedState
 *     On Resume -> RunningState
 *     On Stop -> IdleState
 *   FinishedState
 *     On Replay -> RunningState
 */
public sealed class TestStateMachine
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
        protected TestStateMachine testStateMachine;

        protected BaseState(Func<Task> onEnter = null, Func<Task> onExit = null)
        {
            this.onEnter = onEnter;
            this.onExit = onExit;
        }

        public void Initialize(TestStateMachine testStateMachine)
            => this.testStateMachine = testStateMachine;

        public Task OnEnter() => onEnter?.Invoke() ?? Task.CompletedTask;
        public Task OnExit() => onExit?.Invoke() ?? Task.CompletedTask;
    }

    public interface IIdleState
    {
        Task<IRunningState> Play();
    }

    public interface IRunningState
    {
        Task<IPausedState> Pause();
        Task<IFinishedState> Finish();
        Task<IIdleState> Stop();
    }

    public interface IPausedState
    {
        Task<IRunningState> Resume();
        Task<IFinishedState> Stop();
    }

    public interface IFinishedState
    {
        Task<IRunningState> Replay();
    }


    public sealed class IdleState : BaseState, IIdleState
    {
        public IdleState(Func<Task> onEnter = null, Func<Task> onExit = null) : base(onEnter, onExit)
        {
        }

        public Task<IRunningState> Play() => testStateMachine.OnPlayFromIdle();
    }

    public class RunningState : BaseState, IRunningState
    {
        public RunningState(Func<Task> onEnter = null, Func<Task> onExit = null) : base(onEnter, onExit)
        {
        }

        public Task<IPausedState> Pause() => testStateMachine.OnPauseFromRunning();

        public Task<IIdleState> Stop() => testStateMachine.OnStopFromRunning();

        public Task<IFinishedState> Finish() => testStateMachine.OnFinishFromRunning();
    }

    public class PausedState : BaseState, IPausedState
    {
        public PausedState(Func<Task> onEnter = null, Func<Task> onExit = null) : base(onEnter, onExit)
        {
        }

        public Task<IRunningState> Resume() => testStateMachine.OnResumeFromPaused();
        public Task<IFinishedState> Stop() => testStateMachine.OnStopFromPaused();
    }

    public class FinishedState : BaseState, IFinishedState
    {
        public FinishedState(Func<Task> onEnter = null, Func<Task> onExit = null) : base(onEnter, onExit)
        {
        }

        public Task<IRunningState> Replay() => testStateMachine.OnReplayFromFinished();
    }

    private BaseState currentState;

    private IdleState idleState;
    private RunningState runningState;
    private PausedState pausedState;
    private FinishedState finishedState;
    public SMStatus Status;

    public TestStateMachine(IdleState idleState, RunningState runningState, PausedState pausedState,
        FinishedState finishedState)
    {
        Status = SMStatus.Idle;

        try
        {
            this.idleState = idleState ?? throw new ArgumentNullException(nameof(idleState));
            this.runningState = runningState ?? throw new ArgumentNullException(nameof(runningState));
            this.pausedState = pausedState ?? throw new ArgumentNullException(nameof(pausedState));
            this.finishedState = finishedState ?? throw new ArgumentNullException(nameof(finishedState));
        }
        catch (Exception)
        {
            Status = SMStatus.Failed;
            throw;
        }

        idleState.Initialize(this);
        runningState.Initialize(this);
        pausedState.Initialize(this);
        finishedState.Initialize(this);
    }

    public async Task<IIdleState> Run()
    {
        CheckStatus(true);

        await Transit(null, idleState);
        return idleState;
    }

    private async Task<IRunningState> OnPlayFromIdle()
    {
        CheckStatus();
        await Transit(idleState, runningState);
        return runningState;
    }

    private async Task<IPausedState> OnPauseFromRunning()
    {
        CheckStatus();
        await Transit(runningState, pausedState);
        return pausedState;
    }

    private async Task<IIdleState> OnStopFromRunning()
    {
        CheckStatus();
        await Transit(runningState, idleState);
        return idleState;
    }

    private async Task<IFinishedState> OnFinishFromRunning()
    {
        CheckStatus();
        await Transit(runningState, finishedState);
        return finishedState;
    }

    private async Task<IRunningState> OnResumeFromPaused()
    {
        CheckStatus();
        await Transit(pausedState, runningState);
        return runningState;
    }

    private async Task<IFinishedState> OnStopFromPaused()
    {
        CheckStatus();
        await Transit(pausedState, finishedState);
        return finishedState;
    }

    private async Task<IRunningState> OnReplayFromFinished()
    {
        CheckStatus();
        await Transit(finishedState, runningState);
        return runningState;
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
}