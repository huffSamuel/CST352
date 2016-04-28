#*******************************************************
# Makefile for CST 352 Lab 3
#
# Author: Samuel Huff
# Filename: Makefile
# Date Created: 3/31/2016
# Modifications:
#
#*******************************************************
CC=stacklc

all: user syscalls

clean: 
	rm -f syscalls.sl
	rm -f syscalls.slb
	rm -f user.sl
	rm -f user.slb
	rm -f final_startup__.sl
	rm -f final_startup__.slb

syscalls:
	$(CC) syscalls.c -boot

user:
	$(CC) user.c

