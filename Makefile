CC=clang++
CFLAGS=-g -Wall --std=c++1z
TARGET:test.out

OBJS=graph.o \
	 net.o

test.out:testapp.o ${OBJS}
	${CC} ${CFLAGS} testapp.o ${OBJS} -o test.out

testapp.o:testapp.cc
	${CC} ${CFLAGS} -c testapp.cc -o testapp.o

graph.o:graph.cc
	${CC} ${CFLAGS} -c -I . graph.cc -o graph.o

net.o:net.cc
	${CC} ${CFLAGS} -c -I . net.cc -o net.o

clean:
	/bin/rm *.o
	/bin/rm *.out
	/bin/rm -r *.dSYM
