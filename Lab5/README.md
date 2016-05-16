#CST352 Lab 5 : Threaded Reader

The purpose of this lab is to create a multi-threaded producer/consumer application. The producer(s) will read from a file (or files)
and place the lines in a queue. The consumer threads will read from the queue and display the line of text along with their thread ID.

## Requirements

1. Producer
  * Read 255 characters at a time.
  * Remove newlines from the end of the string.
  * Filter out empty lines (whitespace lines).
  * Place line in shared queue.
2. Consumer
  * Read line from shared queue.
  * Print thread ID along with string.
3. Threading
  * Thread-safe code should be implemented with condition variables or semaphores
  * Synchronization should take place in the queue implementation. It should be invisible to consumer code.
4. Additional
  * No global variables.
  * Make cod bulletproof. Check everything for errors and handle them.
  * Allow multiple producers/consumers.
  * No memory leaks.
  
## Grades and Notes

Lab Score: xx/100

* Waiting for input
