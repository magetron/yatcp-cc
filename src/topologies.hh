#ifndef TOPOLOGIES_CC
#define TOPOLOGIES_CC

#include "graph.hh"

graph_t *hello_world_topo () {

  /*
                        +----------+
                      0/4 |          |0/0
         +----------------+   R0_re  +---------------------------+
         |     4.1.1.1/24 | 127.0.0.1| 2.1.1.1/24                |
         |                +----------+                           |
         |                                                       |
         |                                                       |
         |                                                       |
         | 4.1.1.2/24                                            | 2.1.1.2/24
         |0/5                                                    |0/1
  +-----+-----+                                              +----+-----+
  |  R2_re    |0/3                                        0/2|          |
  |           +----------------------------------------------+  R1_re   |
  | 127.0.0.1 | 3.1.1.2/24                        3.1.1.1/24 | 192.0.1.2|
  +-----+-----+                                              +----------+
   */

  graph_t *t = create_new_graph((char *)"Hello World Topo");
  node_t *R0_re = create_graph_node(t, (char *)"R0_re");
  node_t *R1_re = create_graph_node(t, (char *)"R1_re");
  node_t *R2_re = create_graph_node(t, (char *)"R2_re");

  insert_link_between_nodes(R0_re, R1_re, (char *)"eth0/0", (char *)"eth0/1", 1);
  insert_link_between_nodes(R1_re, R2_re, (char *)"eth0/2", (char *)"eth0/3", 1);
  insert_link_between_nodes(R0_re, R2_re, (char *)"eth0/4", (char *)"eth0/5", 1);

  node_set_loopback_addr(R0_re, new ip_addr_t(127,0,0,1), 32);
  node_set_intf_ip_addr(R0_re, (char *)"eth0/4", new ip_addr_t(4,1,1,1), 24);
  node_set_intf_ip_addr(R0_re, (char *)"eth0/0", new ip_addr_t(2,1,1,1), 24);

  node_set_loopback_addr(R1_re, new ip_addr_t(192,0,1,2), 32);
  node_set_intf_ip_addr(R1_re, (char *)"eth0/1", new ip_addr_t(2,1,1,2), 24);
  node_set_intf_ip_addr(R1_re, (char *)"eth0/2", new ip_addr_t(3,1,1,1), 24);

  node_set_loopback_addr(R2_re, new ip_addr_t(127,0,0,1), 16);
  node_set_intf_ip_addr(R2_re, (char *)"eth0/3", new ip_addr_t(3,1,1,2), 24);
  node_set_intf_ip_addr(R2_re, (char *)"eth0/5", new ip_addr_t(4,1,1,2), 24);

  return t;
}

graph_t *linear_topo () {

  /*
   +-----------+                         +-----------+                         +-----------+
   |           | 0/1                 0/2 |           | 0/3                0/4  |           |
   +     H1    +-------------------------+     H2    +-------------------------+    H3     |
   | 122.1.1.1 | 10.1.1.1/24 10.1.1.2/24 | 122.1.1.2 | 20.1.1.2/24 20.1.1.1/24 | 122.1.1.3 |
   +-----------+                         +-----------+                         +-----------+

   */

  graph_t *t = create_new_graph((char *)"Linear Topo");
  node_t *H1 = create_graph_node(t, (char *)"H1");
  node_t *H2 = create_graph_node(t, (char *)"H2");
  node_t *H3 = create_graph_node(t, (char *)"H3");

  insert_link_between_nodes(H1, H2, (char *)"eth0/1", (char *)"eth0/2", 1);
  insert_link_between_nodes(H2, H3, (char *)"eth0/3", (char *)"eth0/4", 1);

  node_set_loopback_addr(H1, new ip_addr_t(122,1,1,1), 32);
  node_set_loopback_addr(H2, new ip_addr_t(122,1,1,2), 32);
  node_set_loopback_addr(H3, new ip_addr_t(122,1,1,3), 32);

  node_set_intf_ip_addr(H1, (char *)"eth0/1", new ip_addr_t(10,1,1,1), 24);
  node_set_intf_ip_addr(H2, (char *)"eth0/2", new ip_addr_t(10,1,1,2), 24);
  node_set_intf_ip_addr(H2, (char *)"eth0/3", new ip_addr_t(20,1,1,2), 24);
  node_set_intf_ip_addr(H3, (char *)"eth0/4", new ip_addr_t(20,1,1,1), 24);

  return t;
}


#endif
