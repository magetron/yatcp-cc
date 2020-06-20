#ifndef GRAPH_HH
#define GRAPH_HH

#include "net.hh"

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
	unsigned short udp_port_number;
	int udp_sock_fd;
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

// METHODS
graph_t *create_new_graph (char *topology_name);

node_t *create_graph_node (graph_t *graph, char *node_name);

void insert_link_between_nodes (node_t *n1, node_t *n2, char *intf1_name, char *intf2_name, unsigned int cost);


// DEBUG
void dump_interface(interface_t *interface);

void dump_node(node_t *node);

void dump_graph(graph_t *graph);

#endif
