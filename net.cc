#ifndef NET_CC
#define NET_CC

#include "graph.hh"

// METHODS IMPL
bool node_set_loopback_addr (node_t *node, ip_addr_t* ip_addr) {
	assert(ip_addr);
	node -> node_nw_props.has_lb_addr_config = true;
	memcpy(NODE_LB_ADDR(node), &ip_addr, IP_ADDR_LENGTH);
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



#endif
