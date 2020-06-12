#ifndef NET_CC
#define NET_CC

#include "graph.hh"

#include <cstdlib>
#include <ctime>

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

void intf_assign_mac_addr (interface_t *intf) {
	node_t *node = intf -> att_node;

	if (!node) return;

	srand(time(nullptr));
	for (int i = 0; i < MAC_ADDR_LENGTH; i++) 
		(intf) -> intf_nw_props.mac_addr.addr[i] = rand() % 256;
}


// DEBUG IMPL
void dump_node_nw_props(node_t *node){

    printf("\nNode Name = %s\n", node -> node_name);
    if(node -> node_nw_props.has_lb_addr_config){
        printf("\t  lo addr : %s/32\n", NODE_LB_ADDR(node));
    }
}

void dump_intf_props(interface_t *intf){

    dump_interface(intf);

    if(intf -> intf_nw_props.has_ip_addr_config){
        printf("\t IP Addr = %s/%u", INTF_IP(intf), intf -> intf_nw_props.mask);
    }
    else{
         printf("\t IP Addr = %s/%u", "Nil", 0);
    }

    printf("\t MAC : %u:%u:%u:%u:%u:%u\n",
        INTF_MAC(intf)[0], INTF_MAC(intf)[1],
        INTF_MAC(intf)[2], INTF_MAC(intf)[3],
        INTF_MAC(intf)[4], INTF_MAC(intf)[5]);
}

void dump_nw_graph(graph_t *graph){

    interface_t *interface;
    
    printf("Topology Name = %s\n", graph->topology_name);

	for (const auto& node : graph -> node_list) { 
        dump_node_nw_props(node);
        for(unsigned int i = 0; i < MAX_INTF_PER_NODE; i++){
            interface = node -> intfs[i];
            if(!interface) break;
            dump_intf_props(interface);
        }
    }
}



#endif
