#ifndef GRAPH_HH
#define GRAPH_HH

#define NODE_NAME_SIZE 16
#define INTF_NAME_SIZE 16
#define MAX_INTF_PER_NODE 8
#define TOPO_NAME_SIZE 32

#include "net.hh"

#include <cassert>
#include <list>

struct interface_t;
struct link_t;
struct node_t;
struct graph_t;

struct interface_t {
	char intf_name[INTF_NAME_SIZE];
	node_t *att_node;
	link_t *link;
	intf_nw_props_t intf_nw_props;
};

struct link_t {
	interface_t intf1;
	interface_t intf2;
	unsigned int cost;
};

struct node_t {
	char node_name[NODE_NAME_SIZE];
	interface_t *intfs[MAX_INTF_PER_NODE];
	node_nw_props_t node_nw_props;
};

struct graph_t {
	char topology_name[TOPO_NAME_SIZE];
	std::list<node_t *> node_list;
};

// HELPER
static inline node_t *get_nbr_node(interface_t *interface) {
	assert(interface -> att_node);
	assert(interface -> link);

	link_t *link = interface -> link;
	if (interface == &(link -> intf1)) return link -> intf2.att_node;
	else return link -> intf1.att_node;
}

static inline int get_node_intf_available_slot (node_t *node) {
	for (unsigned int i = 0; i < MAX_INTF_PER_NODE; i++) 
		if (node -> intfs[i] == nullptr) return i;
	return -1;
}

static inline interface_t *get_node_intf_by_name (node_t *node, char *intf_name) {
	for (unsigned int i = 0; i < MAX_INTF_PER_NODE; i++) {
		if (!node -> intfs[i]) return nullptr;
		if (strncmp(node -> intfs[i] -> intf_name, intf_name, INTF_NAME_SIZE) == 0) return node -> intfs[i];
	}
	return nullptr;
}

static inline node_t *get_node_by_node_name (graph_t *topo, char *node_name) {
	for (auto& node : topo -> node_list)
		if (strncmp(node -> node_name, node_name, NODE_NAME_SIZE) == 0) return node;
	return nullptr;
}

// GRAPH METHODS
graph_t *create_new_graph (char *topology_name) {
	
	graph_t *graph = new graph_t(); 
	strncpy(graph -> topology_name, topology_name, TOPO_NAME_SIZE);
	graph -> topology_name[TOPO_NAME_SIZE - 1] = '\0';

	return graph;
}

node_t *create_graph_node (graph_t *graph, char *node_name) {
	node_t *node = new node_t();
	strncpy(node -> node_name, node_name, NODE_NAME_SIZE);
	node -> node_name[NODE_NAME_SIZE - 1] = '\0';
	(graph -> node_list).push_back(node);
	return node;
}

void insert_link_between_nodes (node_t *n1, node_t *n2, char *intf1_name, char *intf2_name, unsigned int cost) {
	link_t *link = new link_t();
	
	strncpy(link -> intf1.intf_name, intf1_name, INTF_NAME_SIZE);
	link -> intf1.intf_name[INTF_NAME_SIZE - 1] = '\0';
	strncpy(link -> intf2.intf_name, intf2_name, INTF_NAME_SIZE);
	link -> intf2.intf_name[INTF_NAME_SIZE - 1] = '\0';

	link -> intf1.link = link;
	link -> intf2.link = link;

	link -> intf1.att_node = n1;
	link -> intf2.att_node = n2;
	link -> cost = cost;

	int empty_intf_slot = -1;

	empty_intf_slot = get_node_intf_available_slot(n1);
	n1 -> intfs[empty_intf_slot] = &link -> intf1;
	
	empty_intf_slot = get_node_intf_available_slot(n2);
	n2 -> intfs[empty_intf_slot] = &link -> intf2;
}

// NET METHODS
bool node_set_loopback_address (node_t *node, ip_addr_t* ip_addr) {
	assert(ip_addr);
	node -> node_nw_props.has_lb_addr_config = true;
	memcpy(NODE_LB_ADDR(node), &ip_addr, IP_ADDR_LENGTH);
	return true;
}

bool node_set_intf_ip_addr(node_t *node, char *local_intf, ip_addr_t* ip_addr, unsigned char mask) {
	interface_t *interface = get_node_intf_by_name(node, local_intf);
	if (!interface) assert(0);

	memcpy(INTF_IP(interface), ip_addr, IP_ADDR_LENGTH);
	interface -> intf_nw_props.mask = mask;
	interface -> intf_nw_props.has_ip_addr_config = true;
	return true;
}

bool node_unset_intf_ip_addr(node_t *node, char *local_intf) {
	interface_t *interface = get_node_intf_by_name(node, local_intf);
	if (!interface) assert(0);
	
	interface -> intf_nw_props.has_ip_addr_config = false;
	interface -> intf_nw_props.mask = 0;
	memset(INTF_IP(interface), 0, IP_ADDR_LENGTH);
	return true;
}

// MAC generator
void intf_assign_mac_address (interface_t *interface) {
	
}


// DEBUG
void dump_interface(interface_t *interface) {
   link_t *link = interface -> link;
   node_t *nbr_node = get_nbr_node(interface);

   printf(" Local Node : %s, Interface Name = %s, Nbr Node %s, cost = %u\n",
            interface -> att_node -> node_name,
            interface -> intf_name, nbr_node -> node_name, link -> cost);
}

void dump_node(node_t *node) {
    interface_t *intf;

    printf("Node Name = %s : \n", node->node_name);

    for(unsigned int i = 0; i < MAX_INTF_PER_NODE; i++){
        intf = node -> intfs[i];
        if(!intf) break;
        dump_interface(intf);
    }
}

void dump_graph(graph_t *graph) {
    printf("Topology Name = %s\n", graph->topology_name);

    for (auto& node : graph -> node_list) dump_node(node);
}

#endif
