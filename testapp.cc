#ifndef TESTAPP_CC
#define TESTAPP_CC

#include "topologies.hh"

int main () {
	graph_t *g = hello_world_topo();
	dump_graph(g);
	return 0;
}

#endif
