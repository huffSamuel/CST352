COPTS=-Wall -g -c -O0 
OBJS=sched.o \
	 queue.o \
	 list.o \

EXES=test1 \
	test2 \
	test3 \
	test4 \
	test5 \

all: $(EXES)

clean:
	rm -f $(OBJS)
	rm -f $(EXES)

test1: test1.c $(OBJS)
	gcc $@.c $(OBJS) -pthread -g -o $@

%: %.c $(OBJS)
	gcc $@.c $(OBJS) -pthread -g -o $@

.c.o:
	gcc $(COPTS) $? -o $@
