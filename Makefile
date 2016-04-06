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
COPTS=-g -o

all: prefilter postfilter process

clean: 
	rm -f prefilter postfilter process

prefilter: prefilter.c
	$(CC) $(COPTS) prefilter prefilter.c

postfilter: postfilter.c
	$(CC) $(COPTS) postfilter postfilter.c

process: process.c
	$(CC) $(COPTS) process process.c

