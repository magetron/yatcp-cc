CC=clang++
CFLAGS=-g -Wall --std=c++1z
TARGET:test.out

OBJS=


test.out:testapp.o ${OBJS}
	${CC} ${CFLAGS} testapp.o ${OBJS} -o test.out

testapp.o:testapp.cc
	${CC} ${CFLAGS} -c testapp.cc -o testapp.o

clean:
	/bin/rm *.o
	/bin/rm *.out
