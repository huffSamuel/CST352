#*************************************************************
# Author:        Philip Howard
# Email:         phil.howard@oit.edu
# Filename:      Makefile
# Date Created:  2016-04-13
#*************************************************************

COPTS = -Wall -std=gnu99 -g -O0 -c -pthread

OBJS = list.o \
       mytest.o \

OBJS2 = list_hand.o \
		myHtest.o \

all: mytest myHtest

clean:
	rm -rf mytest
	rm -rf myHtest
	rm -rf $(OBJS)

.c.o:
	gcc $(COPTS) $? -o $@

mytest: mytest.c $(OBJS)
	gcc $(OBJS) -o mytest -pthread

myHtest: myHtest.c $(OBJS2)
	gcc $(OBJS2) -o myHtest -pthread
