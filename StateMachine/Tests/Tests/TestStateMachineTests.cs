using Xunit;

namespace StateMachineTestApp.Tests
{
    public class TestStateMachineTests
    {
        private TestStateMachine CreateSyncStateMachine(Action<string> logCallback = null)
        {
            if (logCallback == null)
            {
                logCallback = (str) => { };
            }

            return new TestStateMachine(
                new TestStateMachine.IdleState(
                    onEnter: async () => logCallback("IdleState entered"),
                    onExit: async () => logCallback("IdleState exited")
                ),
                new TestStateMachine.RunningState(
                    onEnter: async () => logCallback("RunningState entered"),
                    onExit: async () => logCallback("RunningState exited")
                ),
                new TestStateMachine.PausedState(
                    onEnter: async () => logCallback("PausedState entered"),
                    onExit: async () => logCallback("PausedState exited")
                ),
                new TestStateMachine.FinishedState(
                    onEnter: async () => logCallback("FinishedState entered"),
                    onExit: async () => logCallback("FinishedState exited")
                ));
        }

        private TestStateMachine CreateAsyncStateMachine(Action<string> logCallback = null)
        {
            if (logCallback == null)
            {
                logCallback = (str) => { };
            }

            return new TestStateMachine(
                new TestStateMachine.IdleState(
                    onEnter: async () =>
                    {
                        logCallback("IdleState entered");
                        await Task.Delay(100);
                    },
                    onExit: async () =>
                    {
                        logCallback("IdleState exited");
                        await Task.Delay(100);
                    }),
                new TestStateMachine.RunningState(
                    onEnter: async () =>
                    {
                        logCallback("RunningState entered");
                        await Task.Delay(100);
                    },
                    onExit: async () =>
                    {
                        logCallback("RunningState exited");
                        await Task.Delay(100);
                    }),
                new TestStateMachine.PausedState(
                    onEnter: async () =>
                    {
                        logCallback("PausedState entered");
                        await Task.Delay(100);
                    },
                    onExit: async () =>
                    {
                        logCallback("PausedState exited");
                        await Task.Delay(100);
                    }),
                new TestStateMachine.FinishedState(
                    onEnter: async () =>
                    {
                        logCallback("FinishedState entered");
                        await Task.Delay(100);
                    },
                    onExit: async () =>
                    {
                        logCallback("FinishedState exited");
                        await Task.Delay(100);
                    }));
        }


        private void CheckLog(List<string> log, string expected)
        {
            if (!log.Any())
            {
                Assert.Fail($"Log is empty, expected: {expected}");
            }

            Assert.Equal(log[0], expected);
            log.RemoveAt(0);
        }

        [Fact]
        public async void FullFlowTransitioningTest()
        {
            List<string> log = new();
            var stateMachine = CreateSyncStateMachine((str) => { log.Add(str); });

            var idle = await stateMachine.Run();

            CheckLog(log, "IdleState entered");

            var running = await idle.Play();

            CheckLog(log, "IdleState exited");
            CheckLog(log, "RunningState entered");

            var paused = await running.Pause();

            CheckLog(log, "RunningState exited");
            CheckLog(log, "PausedState entered");

            running = await paused.Resume();

            CheckLog(log, "PausedState exited");
            CheckLog(log, "RunningState entered");

            var finished = await running.Finish();

            CheckLog(log, "RunningState exited");
            CheckLog(log, "FinishedState entered");

            await finished.Replay();

            CheckLog(log, "FinishedState exited");
            CheckLog(log, "RunningState entered");

            Assert.False(log.Any());
        }

        [Fact]
        public async Task InitialState()
        {
            var stateMachine = CreateSyncStateMachine();
            Assert.Equal(TestStateMachine.SMStatus.Idle, stateMachine.Status);
        }

        [Fact]
        public async Task RunOnce()
        {
            var stateMachine = CreateSyncStateMachine();
            Assert.Equal(TestStateMachine.SMStatus.Idle, stateMachine.Status);
            await stateMachine.Run();
            Assert.Equal(TestStateMachine.SMStatus.Running, stateMachine.Status);
        }

        [Fact]
        public async Task RunTwice_GetError()
        {
            var stateMachine = CreateSyncStateMachine();
            await stateMachine.Run();

            await Assert.ThrowsAsync<InvalidOperationException>(async () =>
            {
                await stateMachine.Run();
                throw new Exception();
            });

            Assert.Equal(TestStateMachine.SMStatus.Failed, stateMachine.Status);
        }

        [Fact]
        public async Task SyncTransition()
        {
            List<string> log = new();
            var stateMachine = CreateSyncStateMachine((str) => { log.Add(str); });
            var idle = await stateMachine.Run();

            CheckLog(log, "IdleState entered");

            await idle.Play();

            CheckLog(log, "IdleState exited");
            CheckLog(log, "RunningState entered");

            Assert.False(log.Any());
        }

        [Fact]
        public async Task AsyncTransition()
        {
            List<string> log = new();
            var stateMachine = CreateAsyncStateMachine((str) => { log.Add(str); });
            var idle = await stateMachine.Run();

            CheckLog(log, "IdleState entered");

            await idle.Play();

            CheckLog(log, "IdleState exited");
            CheckLog(log, "RunningState entered");

            Assert.False(log.Any());
        }

        [Fact]
        public async Task ExceptionInOnExit()
        {
            var stateMachine = new TestStateMachine(
                new TestStateMachine.IdleState(
                    onEnter: async () => Console.WriteLine("IdleState entered"),
                    onExit: async () => throw new StackOverflowException("Exit exception")
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

            TestStateMachine.IIdleState idleState = await stateMachine.Run();
            await Assert.ThrowsAsync<StackOverflowException>(async () => await idleState.Play());
        }

        [Fact]
        public async Task ExceptionInOnEnter()
        {
            var stateMachine = new TestStateMachine(
                new TestStateMachine.IdleState(
                    onEnter: async () => Console.WriteLine("IdleState entered"),
                    onExit: async () => Console.WriteLine("IdleState exited")
                ),
                new TestStateMachine.RunningState(
                    onEnter: async () => throw new StackOverflowException("Enter exception"),
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

            TestStateMachine.IIdleState idleState = await stateMachine.Run();
            await Assert.ThrowsAsync<StackOverflowException>(async () => await idleState.Play());
        }

        [Fact]
        public async Task TriggerWrongState_GetException()
        {
            var stateMachine = CreateSyncStateMachine();
            TestStateMachine.IIdleState idleState = await stateMachine.Run();
            await idleState.Play();
            await Assert.ThrowsAsync<InvalidOperationException>(() => idleState.Play());
        }

        [Fact]
        public async Task TriggerTransitionWhileTransitioning_GetException()
        {
            var stateMachine = CreateSyncStateMachine();
            TestStateMachine.IIdleState idleState = await stateMachine.Run();
            idleState.Play();
            await Assert.ThrowsAsync<InvalidOperationException>(() => idleState.Play());
        }

        [Fact]
        public async Task TestStatusUpdates()
        {
            var stateMachine = CreateAsyncStateMachine();
            Assert.Equal(TestStateMachine.SMStatus.Idle, stateMachine.Status);

            Task<TestStateMachine.IIdleState> task1 = stateMachine.Run();
            Assert.Equal(TestStateMachine.SMStatus.Transitioning, stateMachine.Status);
            var idle = await task1;
            Assert.Equal(TestStateMachine.SMStatus.Running, stateMachine.Status);

            var task2 = idle.Play();
            Assert.Equal(TestStateMachine.SMStatus.Transitioning, stateMachine.Status);
            var running = await task2;
            Assert.Equal(TestStateMachine.SMStatus.Running, stateMachine.Status);

            var task3 = running.Pause();
            Assert.Equal(TestStateMachine.SMStatus.Transitioning, stateMachine.Status);
            var paused = await task3;
            Assert.Equal(TestStateMachine.SMStatus.Running, stateMachine.Status);

            var task4 = paused.Resume();
            Assert.Equal(TestStateMachine.SMStatus.Transitioning, stateMachine.Status);
            var resumed = await task4;
            Assert.Equal(TestStateMachine.SMStatus.Running, stateMachine.Status);

            var task5 = resumed.Finish();
            Assert.Equal(TestStateMachine.SMStatus.Transitioning, stateMachine.Status);
            var finished = await task5;
            Assert.Equal(TestStateMachine.SMStatus.Running, stateMachine.Status);

            var task6 = finished.Replay();
            Assert.Equal(TestStateMachine.SMStatus.Transitioning, stateMachine.Status);
            await task6;
            Assert.Equal(TestStateMachine.SMStatus.Running, stateMachine.Status);
        }
    }
}