#ifndef GRAPH_HH
#define GRAPH_HH

#define NODE_NAME_SIZE 16
#define INTF_NAME_SIZE 16
#define MAX_INTF_PER_NODE 8
#define TOPO_NAME_SIZE 32

#include<assert.h>
#include<list>

struct interface_t;
struct link_t;
struct node_t;
struct graph_t;

struct interface_t {
	char intf_name[INTF_NAME_SIZE];
	node_t *att_node;
	link_t *link;
};

struct link_t {
	interface_t intf1;
	interface_t intf2;
	unsigned int cost;
};

struct node_t {
	char node_name[NODE_NAME_SIZE];
	interface_t *intfs[MAX_INTF_PER_NODE];
};

struct graph_t {
	char topology_name[TOPO_NAME_SIZE];
	std::list<node_t> node_list;
};

static inline node_t *get_nbr_node(interface_t *interface) {
	assert(interface -> att_node);
	assert(interface -> link);

	link_t *link = interface -> link;
	if (interface == &link -> intf1) &link -> intf2.att_node;
	else return &link -> intf1.att_node;
}

static inline int get_node_intf_available_slot (node_t *node) {
	for (int i = 0; i < MAX_INTF_PER_NODE; i++) 
		if (node -> intfs[i] == nullptr) return i;
	return -1;
}



#endif
