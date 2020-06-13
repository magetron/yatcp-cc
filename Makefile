CC=clang++
CCDEBUGFLAGS=-g -Wall --std=c++1z
CCFLAGS=-O2 -Wall --std=c++1z
TARGET:test.out

OBJS=graph.o \
	 net.o

test.out:testapp.o ${OBJS}
	${CC} ${CCDEBUGFLAGS} testapp.o ${OBJS} -o test.out

testapp.o:testapp.cc
	${CC} ${CCDEBUGFLAGS} -c testapp.cc -o testapp.o

graph.o:graph.cc
	${CC} ${CCDEBUGFLAGS} -c -I . graph.cc -o graph.o

net.o:net.cc
	${CC} ${CCDEBUGFLAGS} -c -I . net.cc -o net.o

clean:
	/bin/rm *.o
	/bin/rm *.out
