# CST352 Lab 1 : Processes and Pipes

The purpose of this lab is to explore process and pipe creation in a Linux environment.

## Requirements

1. Write a program named *prefilter* that reads lines of text from stdin and for each line prints to stdout all characters including and following a specified delimiter character.
2. Write a program named *postfilter* that reads lines of text from stdin and for each line prints to stdout all characters including and previous to a specified delmiter character.
3. Write a program called *process* that will use *prefilter* and *postfilter* to prices the output of a specified command.
4. Supply a makefile to build executables. It must include the targets *all* and *clean*.

## Grade and Notes

Scored 80/100 points on the lab. Feedback:
-  It appears like you postfilter removes lines that did not have the filter character. It should have kept them.
-  based on stderr, your prefilter always finishes before your postfilter starts.
-  Indentation is supposed to be set to 3 or 4 characters, not 2.
-  In process.c, main() should be broken into smaller functions.
