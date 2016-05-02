#CST352 Lab 4 : Threads

The purpose of this lab is to understand threads and making C code thread-safe.

## Requirements

1. Write a multi-threaded program that satisfies these requirements:
  * main() should have an argument that specifies the number of threads to run.
  * main() should start the threads and use `pthread_join` to wait for them to finish.
  * The struct passed to `perform_queue_operations`  should have an additional value that is the thread number.
  * Number of items specified in the command line is the number of items to be added to the list.
2. Thread-safe the linked list with single lock.
3. Thread-safe the linked list with hand-over-hand locking.
4. Test the performance difference between the two locking methods and answer these questions:
  * Which version is faster and by how much? Why?
  * Are there parameter settings for which the other version is faster? What are they or why not?
  * How would you expect the number of available processors to affect your results?
  * Which of the numbers reported by `time` are the most meaningful for this type of test? Why?
  * Under what conditions would hand-over-hand locking be of benefit and under what conditions would you expect it to not be beneficial?
  
## Grades and Nodes

Lab Score: xx/100

* Waiting for input
