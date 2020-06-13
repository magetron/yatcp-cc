CC=clang++
CCDEBUGFLAGS=-g -Wall --std=c++1z
CCFLAGS=-O2 -Wall --std=c++1z
TARGET:test.out

OBJS=objs/graph.o \
	 objs/net.o	 \
	 objs/utils.o

test.out:objs/testapp.o ${OBJS}
	${CC} ${CCDEBUGFLAGS} objs/testapp.o ${OBJS} -o test.out

objs/testapp.o:src/testapp.cc
	${CC} ${CCDEBUGFLAGS} -c src/testapp.cc -o objs/testapp.o

objs/graph.o:src/graph.cc
	${CC} ${CCDEBUGFLAGS} -c -I . src/graph.cc -o objs/graph.o

objs/net.o:src/net.cc
	${CC} ${CCDEBUGFLAGS} -c -I . src/net.cc -o objs/net.o

objs/utils.o:src/utils.cc
	${CC} ${CCDEBUGFLAGS} -c -I . src/utils.cc -o objs/utils.o

clean:
	/bin/rm -r objs/
	/bin/rm *.out
	mkdir objs
