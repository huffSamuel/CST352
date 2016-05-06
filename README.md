This is my repository for code for CST352 - Operating Systems taken at Oregon Institute of Technology.

## Class Description:
Issues in Operating Systems Design. Topics include: 
- processes
- threads and fibers
- privilege modes
- preemptive multitasking
- process state machine
- scheduling paradigms
- system calls/traps
- shared resources and synchronization primitives
- memory management schemes/ virtual memory
- deadlock detection
- handling and avoidance
- I/O management

### Requirements for labs:
All labs are required to be "paranoid" checked. This is where the student (programmer) assumes that underlying OS functions
will fail, so all returns must be checked. 
For example: printf("Hello World") returns the number of characters it printed. If it doesn't return a valid number we should handle the error.
```
if(printf("Hello world") <= 0)
{
  // Error detected with printf
  if( fprintf(STDERR, "Failed to print to STDOUT"); <= 0)
    exit(5); // fprintf failed. 
  exit(4);   // printf failed.
}
```

### Labs to date:
- Lab 1: Pipes and Processes
- Lab 2: Memory Allocation
- Lab 3: System Calls in custom STACKL environment
- Lab 4: Threads
- Lab 5: Producer/Consumer
