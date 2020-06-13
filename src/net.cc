#ifndef NET_CC
#define NET_CC

#include "graph.hh"

#include <cstdlib>
#include <iostream>

using namespace std;

// METHODS IMPL
bool node_set_loopback_addr (node_t *node, ip_addr_t* ip_addr, unsigned char mask) {
	assert(ip_addr);
	node -> node_nw_props.has_lb_addr_config = true;
	memcpy(NODE_LB_ADDR(node), ip_addr, IP_ADDR_LENGTH);
	return true;
}

bool node_set_intf_ip_addr (node_t *node, char *local_intf, ip_addr_t *ip_addr, unsigned char mask) {
	interface_t *interface = get_node_intf_by_name(node, local_intf);
	if (!interface) assert(0);

	memcpy(INTF_IP(interface), ip_addr, IP_ADDR_LENGTH);
	interface -> intf_nw_props.mask = mask;
	interface -> intf_nw_props.has_ip_addr_config = true;
	return true;
}

bool node_unset_intf_ip_addr (node_t *node, char *local_intf) {
	interface_t *interface = get_node_intf_by_name(node, local_intf);
	if (!interface) assert(0);
	
	interface -> intf_nw_props.has_ip_addr_config = false;
	interface -> intf_nw_props.mask = 0;
	memset(INTF_IP(interface), 0, IP_ADDR_LENGTH);
	return true;
}

interface_t *node_get_matching_subnet_intf (node_t *node, ip_addr_t *ip_addr) {
	for (unsigned int i = 0; i < MAX_INTF_PER_NODE; i++)
		if (!node -> intfs[i]) return nullptr;
		else if (IS_INTF_L3_MODE(node -> intfs[i]) && (node -> intfs[i]) -> intf_nw_props.ip_addr == *ip_addr) return node -> intfs[i];
	return nullptr;
}		

// HASH MAC ADDR GENERATION - should assign according to manufacturer and serial number in real-life scenarios
void intf_assign_mac_addr (interface_t *intf) {
	node_t *node = intf -> att_node;

	if (!node) return;

	srand((unsigned long)intf);

	for (int i = 0; i < MAC_ADDR_LENGTH; i++) 
		(intf) -> intf_nw_props.mac_addr.addr[i] = rand() % 256;
}


// DEBUG IMPL
void dump_node_nw_props (node_t *node) {

    printf("\nNode Name = %s\n", node -> node_name);
    if(node -> node_nw_props.has_lb_addr_config)
		printf("\t  loopback addr : %u.%u.%u.%u/%u\n", NODE_LB_ADDR(node)[0], NODE_LB_ADDR(node)[1], NODE_LB_ADDR(node)[2], NODE_LB_ADDR(node)[3], node -> node_nw_props.mask);
}

void dump_intf_props (interface_t *intf) {

    dump_interface(intf);

    if(intf -> intf_nw_props.has_ip_addr_config)
		printf("\t IP Addr = %u.%u.%u.%u/%u", INTF_IP(intf)[0], INTF_IP(intf)[1], INTF_IP(intf)[2], INTF_IP(intf)[3], intf -> intf_nw_props.mask);
	else
		printf("\t IP Addr = %s/%u", "NULL", 0);

    printf("\t MAC : %02X:%02X:%02X:%02X:%02X:%02X\n",
        INTF_MAC(intf)[0], INTF_MAC(intf)[1],
        INTF_MAC(intf)[2], INTF_MAC(intf)[3],
        INTF_MAC(intf)[4], INTF_MAC(intf)[5]);
}

void dump_nw_graph (graph_t *graph) {

    printf("Topology Name = %s\n", graph->topology_name);

	for (const auto& node : graph -> node_list) { 
        dump_node_nw_props(node);
        for(unsigned int i = 0; i < MAX_INTF_PER_NODE; i++){
            interface_t *interface = node -> intfs[i];
            if(!interface) break;
            dump_intf_props(interface);
        }
    }
}



#endif
