#ifndef LAYER2_CC
#define LAYER2_CC

#include "layer2.hh"

extern cli_def *cli;

// TODO

// METHOD IMPL
void l2_frame_recv (node_t *node, interface_t *intf, unsigned char *pkt, unsigned int pkt_size) {

}

void send_arp_broadcast_request (node_t *node, interface_t *o_intf, ip_addr_t *ip) {
	cli_print(cli, "Sending ARP broadcast request ... node name = %s, ip_addr = %u.%u.%u.%u", node -> node_name, /*o_intf -> intf_name,*/ ip -> addr[0], ip -> addr[1], ip -> addr[2], ip -> addr[3]);
	return;
}


// DEBUG IMPL
void dump_arp_table (arp_table_t *arp_table) {
	assert(arp_table);
	for (auto p : (*arp_table).map) 
		cli_print(cli, "IP : %u.%u.%u.%u,  MAC : %02X.%02X.%02X.%02X.%02X.%02X, O_INTF = %s\n",
				p.first.addr[0], p.first.addr[1], p.first.addr[2], p.first.addr[3],
				p.second.mac_addr.addr[0], p.second.mac_addr.addr[1], p.second.mac_addr.addr[2], p.second.mac_addr.addr[3], p.second.mac_addr.addr[4], p.second.mac_addr.addr[5],
				p.second.o_intf_name);
}


#endif
