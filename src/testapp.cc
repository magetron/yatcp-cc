#ifndef TESTAPP_CC
#define TESTAPP_CC

#include "graph.hh"
#include "nwcli.hh"
#include "topologies.hh"
#include "utils.hh"

graph_t *topo = nullptr;

int main (int argc, char **argv) {
	topo = hello_world_topo();
	//dump_nw_graph(topo);
	init_nwcli();
	serve_nwcli();
	return 0;
}

#endif
