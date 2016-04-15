#*******************************************************
# Makefile for CST 352 Lab 1
#
# Author: Samuel Huff
# Filename: Makefile
# Date Created: 3/31/2016
# Modifications:
#
#*******************************************************
CC=gcc
COPTS=-Wall -g -O0 -o 

all: main

clean: 
	rm -f main
	rm -f *.o

main:
	$(CC) $(COPTS) main main.c bucket_t.c buddy.c
