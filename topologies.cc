#ifndef TOPOLOGIES_CC
#define TOPOLOGIES_CC

#include "graph.hh"

graph_t *hello_world_topo () {

	/*
                          +----------+
                      0/4 |          |0/0
         +----------------+   R0_re  +---------------------------+
         |                |          |                           |
         |                +----------+                           |
         |                                                       |
         |                                                       |
         |                                                       |
         |                                                       |
         |0/5                                                    |0/1
     +---+---+                                              +----+-----+
     |       |0/3                                        0/2|          |
     | R2_re +----------------------------------------------+    R1_re |
     |       |                                              |          |
     +-------+                                              +----------+

	 */

	graph_t *t = create_new_graph((char *)"Hello World Graph");
	node_t *R0_re = create_graph_node(t, (char *)"R0_re");
	node_t *R1_re = create_graph_node(t, (char *)"R1_re");
	node_t *R2_re = create_graph_node(t, (char *)"R2_re");

	insert_link_between_nodes(R0_re, R1_re, (char *)"eth0/0", (char *)"eth0/1", 1);
	insert_link_between_nodes(R1_re, R2_re, (char *)"eth0/2", (char *)"eth0/3", 1);
	insert_link_between_nodes(R0_re, R2_re, (char *)"eth0/4", (char *)"eth0/5", 1);

	return t;
}

/*
int main () {
	graph_t *g = hello_world_topo();
	dump_graph(g);
	return 0;
}
*/	

#endif
