#CST352 Lab 6: User Mode Threads

In this lab we constructed a cooperative 1:N multithreaded system. Demo code was supplied that performed the machine-specific stack manipulations. Queue and List code was obtained from previous labs.

**Note** The stack manipulation code is machine specific and not guaranteed to work on all machines. It is tested as-is with x86_64 machines running Linux and compiled with gcc 4.8.2.

## Requirements

####Step 1: Running a Single Thread
1. Switch to a newly created thred. It does NOT need to return to main.

####Step 2: Running Multiple Threads
1. Implement a queue to track all running and ready-to-run threads.
2. Threads need to switch with other threads that are waiting to run.

####Step 3: Handle Completed Threads
1. Allow threads to clean up their TCB when completed (don't distinguish between joined and detached threads yet).
2. There should be no memory leaks from this point forward.

####Step 4: Joining Threads
1. Allow thread A to wait for threads B, C, etc... to completed.
2. Detached threads should not block parent threads.
3. Implement a list to track all current threads (running, ready-to-run, waiting).

####Step 5: Returning Values from Threads
1. Allow threads to return values to parent threads.

## Grade and Notes

78/100

* Your build had warnings.
* You don't need to call yield() in create(). Let the thread creating the other keep the CPU. Also don't need to call yield() after a join().
* valgrind reports memory leaks for some combinations and orderings of join, exit, detach
* File scope variables should be static
* You didn't validate the return value for malloc() for stacks.
* In exit(), you queue the waiting thread without making sure there IS a waiting thread.
* The write-up had the following rule for join: "When a thread calls mythread_join() after the other thread finishes, the calling thread can return without yielding the CPU.". Your implementation did not follow this rule.
