CC=g++
CCFLAGS=-g -fsanitize=address -Wall -Wno-class-memaccess -Wno-unused-but-set-variable -Wno-unused-function -Wno-unused-variable --std=c++1z
TARGET:test.out
LIBS= -pthread -lcrypt ./libs/libcli/libcli.a
OBJS=objs/graph.o \
	 objs/net.o	 \
	 objs/nwcli.o \
	 objs/comm.o \
	 objs/pktdump.o \
	 objs/layer2/layer2.o

test.out:init objs/testapp.o ${OBJS}
	${CC} ${CCFLAGS} objs/testapp.o ${OBJS} -o test.out ${LIBS}

init:
	mkdir -p objs/layer2

objs/testapp.o:src/testapp.cc
	${CC} ${CCFLAGS} -c src/testapp.cc -o objs/testapp.o

objs/graph.o:src/graph.cc
	${CC} ${CCFLAGS} -c -I . src/graph.cc -o objs/graph.o

objs/net.o:src/net.cc
	${CC} ${CCFLAGS} -c -I . src/net.cc -o objs/net.o

objs/nwcli.o:src/nwcli.cc
	${CC} ${CCFLAGS} -c -I . src/nwcli.cc -o objs/nwcli.o

objs/pktdump.o:src/pktdump.cc
	${CC} ${CCFLAGS} -c -I . src/pktdump.cc -o objs/pktdump.o

objs/comm.o:src/comm.cc
	${CC} ${CCFLAGS} -c -I . src/comm.cc -o objs/comm.o

objs/layer2/layer2.o:src/layer2/layer2.cc
	${CC} ${CCFLAGS} -c -I . src/layer2/layer2.cc -o objs/layer2/layer2.o

libs/libcli/libcli.so.1.10:
	(cd libs/libcli; make)
	cp libs/libcli/libcli.so.1.10 .

clean:
	/bin/rm -f *.out
	/bin/rm -f libcli.so.1.10
	/bin/rm -rf objs/

all:
	(cd libs/libcli; make)
	make

cleanall:
	make clean
	(cd libs/libcli; make clean)
