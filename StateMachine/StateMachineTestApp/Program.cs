// Example of using state machine

using StateMachineTestApp;

Console.WriteLine("Program started");

var stateMachine = new TestStateMachine(
    new TestStateMachine.IdleState(
        onEnter: async () => Console.WriteLine("IdleState entered"),
        onExit: async () => Console.WriteLine("IdleState exited")
    ),
    new TestStateMachine.RunningState(
        onEnter: async () => Console.WriteLine("RunningState entered"),
        onExit: async () => Console.WriteLine("RunningState exited")
    ),
    new TestStateMachine.PausedState(
        onEnter: async () => Console.WriteLine("PausedState entered"),
        onExit: async () => Console.WriteLine("PausedState exited")
    ),
    new TestStateMachine.FinishedState(
        onEnter: async () => Console.WriteLine("FinishedState entered"),
        onExit: async () => Console.WriteLine("FinishedState exited")
    ));

var idle = await stateMachine.Run();
var running = await idle.Play();
var paused = await running.Pause();
running = await paused.Resume();
var finished = await running.Finish();
var runningState = await finished.Replay();