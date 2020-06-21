#ifndef TESTAPP_CC
#define TESTAPP_CC

#include "config.hh"
#include "import.hh"

#include "graph.hh"
#include "topologies.hh"
#include "comm.hh"
#include "nwcli.hh"

graph_t *topo = nullptr;

static inline void network_sim_test () {
	topo = hello_world_topo();

	nw_start_pkt_receiver_thread(topo);
	
	//wait for pkt_receiver to start
	sleep(1);

	node_t *send_node = get_node_by_node_name(topo, "R0_re");
	interface_t *send_intf = get_node_intf_by_name(send_node, "eth0/0");

	char msg[] = "Hello World!";
	
	send_pkt(msg, strlen(msg), send_intf);
	send_pkt_flood(send_node, nullptr, msg, strlen(msg));
}

static inline void arp_impl_test () {
	topo = linear_topo();

	nw_start_pkt_receiver_thread(topo);
	
	//wait for pkt_receiver to start
	sleep(1);

	init_nwcli();
	serve_nwcli();
}


int main (int argc, char **argv) {
	arp_impl_test();
	return 0;
}

#endif
