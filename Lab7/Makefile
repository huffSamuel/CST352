COPTS=-Wall -g -c  -O0 
OBJS=myfs.o \
	fstest.o

all: fstest

clean:
	rm -f $(OBJS)
	rm -f fstest
	rm -f out

.c.o:
	gcc $(COPTS) $? -o $@

fstest: $(OBJS)
	gcc $(OBJS) -g -o fstest

