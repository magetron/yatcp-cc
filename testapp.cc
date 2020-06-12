#ifndef TESTAPP_CC
#define TESTAPP_CC

#include "graph.hh"
#include "topologies.hh"

int main () {
	graph_t *g = hello_world_topo();
	dump_nw_graph(g);
	return 0;
}

#endif
