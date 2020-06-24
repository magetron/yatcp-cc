#ifndef TESTAPP_CC
#define TESTAPP_CC

#include "config.hh"
#include "import.hh"

#include "graph.hh"
#include "topologies.hh"
#include "comm.hh"
#include "nwcli.hh"
#include "layer2/layer2.hh"

graph_t *topo = nullptr;

static inline void network_sim_test () {
	topo = hello_world_topo();

	nw_start_pkt_receiver_thread(topo);
	
	//wait for pkt_receiver to start
	sleep(1);

	node_t *send_node = get_node_by_node_name(topo, (char *)"R0_re");
	interface_t *send_intf = get_node_intf_by_name(send_node, (char *)"eth0/0");

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

static inline void check_size () {
	printf("ip_addr_t size = %lu, mac_addr_t size = %lu, ethernet_hdr_t size = %lu, ethernet_hdr_t excluding payload size = %lu, arp_hdr_t size = %lu\n", sizeof(ip_addr_t), sizeof(mac_addr_t), sizeof(ethernet_hdr_t), ETH_HDR_SIZE_EXCL_PAYLOAD, sizeof(arp_hdr_t));
}

static inline void ethernet_hdr_check () {
	char msg[] = "Hello World!Hello World!Hello World!Hello World!";
	unsigned short msg_size = 49;
	ethernet_hdr_t *hdr = new ethernet_hdr_t(new mac_addr_t(1,1,1,1,1,1), new mac_addr_t(2,2,2,2,2,2), (unsigned char *)&msg[0], msg_size);
	void* plain_hdr = hdr -> get_hdr();
	for (unsigned int i = 0; i < ETH_HDR_SIZE_EXCL_PAYLOAD + msg_size; i++) 
		printf("%02X ", *((unsigned char *)plain_hdr + i));
	printf("\n");
	delete hdr;
	ethernet_hdr_t *n_hdr = new ethernet_hdr_t();
	n_hdr -> restore_ethernet_hdr_t(plain_hdr);
	printf("recovered payload = %s, ", n_hdr -> payload);
	printf("recovered msg size = %hu\n", n_hdr -> length);
}

int main (int argc, char **argv) {
	check_size();
	ethernet_hdr_check();
	arp_impl_test();
	return 0;
}

#endif
