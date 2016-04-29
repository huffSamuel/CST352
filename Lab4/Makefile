#*************************************************************
# Author:        Philip Howard
# Email:         phil.howard@oit.edu
# Filename:      Makefile
# Date Created:  2016-04-13
#*************************************************************

COPTS = -Wall -g -O0 -c -pthread

OBJS = list.o \
       mytest.o \

all: mytest

clean:
	rm -rf mytest
	rm -rf $(OBJS)

.c.o:
	gcc $(COPTS) $? -o $@

mytest: mytest.c $(OBJS)
	gcc $(OBJS) -o mytest -pthread
