CC=clang++
CCDEBUGFLAGS=-g -fsanitize=address -Wall --std=c++1z
CCFLAGS=-O2 -Wall --std=c++1z
TARGET:test.out
LIBS= -pthread -L ./libs/libcli -lcli
OBJS=objs/graph.o \
	 objs/net.o	 \
	 objs/nwcli.o \
	 objs/comm.o \
	 objs/layer2/layer2.o

test.out:init objs/testapp.o ${OBJS} libs/libcli/libcli.so.1.10
	${CC} ${CCDEBUGFLAGS} objs/testapp.o ${OBJS} -o test.out ${LIBS}

init:
	mkdir -p objs/layer2

objs/testapp.o:src/testapp.cc
	${CC} ${CCDEBUGFLAGS} -c src/testapp.cc -o objs/testapp.o

objs/graph.o:src/graph.cc
	${CC} ${CCDEBUGFLAGS} -c -I . src/graph.cc -o objs/graph.o

objs/net.o:src/net.cc
	${CC} ${CCDEBUGFLAGS} -c -I . src/net.cc -o objs/net.o

objs/nwcli.o:src/nwcli.cc
	${CC} ${CCDEBUGFLAGS} -c -I . src/nwcli.cc -o objs/nwcli.o

objs/comm.o:src/comm.cc
	${CC} ${CCDEBUGFLAGS} -c -I . src/comm.cc -o objs/comm.o

objs/layer2/layer2.o:src/layer2/layer2.cc
	${CC} ${CCDEBUGFLAGS} -c -I . src/layer2/layer2.cc -o objs/layer2/layer2.o


libs/libcli/libcli.so.1.10:
	(cd libs/libcli; make)
	cp libs/libcli/libcli.so.1.10 .

clean:
	/bin/rm -f *.out
	/bin/rm -f libcli.so.1.10
	/bin/rm -rf objs/

all:
	make
	(cd libs/libcli; make)

cleanall:
	make clean
	(cd libs/libcli; make clean)
