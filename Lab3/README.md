#CST352 Lab 3 : Syscalls

The purpose of this lab is to understand the interface between user and Operating System by writing code in the stackl environment.

## Requirements

1. Write a user mode library featuring these functions:
  * int prints(char * string) - Prints a string to STDOUT. Returns 0 indicating success.
  * int printi(int value) - Prints an integer to STDOUT. Returns 0 indicating success.
  * int geti() - Reads an integer from STDIN and returns the integer.
  * int gets(char * buff) - Reads a line of text from STDIN. Behaves like `fgets(buff, 256, stdin)`
  * int halt() - Causes the CPU to halt (terminates the stackl interpreter)
2. Implement a single syscall function that validates arguments and executes a TRAP to complete the syscall.
3. Implement a single systrap function that is executed when the TRAP function is called. It should validate arguments and perform the actual I/O.
 
## Stackl
Stackl is a stack-based virtual environment developed for teaching students about operating systems. It allows coding in a C-like language to write interrupt service routines, syscalls, and operating system code without requiring actual hardware.

Stackl and its documentation can be obtained [here](https://github.com/philip-w-howard/stackl)

## Grade and Notes
Pending...
