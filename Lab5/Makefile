COPTS = -Wall -g -O0 -c -pthread

OBJS = prod_cons.o \
       queue.o \
       main.o \

all: mytest

clean:
	rm -rf mytest
	rm -rf $(OBJS)

.c.o:
	gcc $(COPTS) $? -o $@

mytest: $(OBJS)
	gcc $(OBJS) -o mytest -pthread
