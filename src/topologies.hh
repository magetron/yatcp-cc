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

graph_t *l2_switch_topo () {
  /*
                                       +-----------+
                                       |  H4       |
                                       | 122.1.1.4 |
                                       +----+------+
                                            |eth0/7 - 10.1.1.3/24
                                            |
                                            |eth0/1
                                       +----+----+                        +---------+
       +---------+                     |         |                        |         |
       |         |10.1.1.2/24          |   L2Sw  |eth0/2       10.1.1.1/24|  H3     |
       |  H1     +---------------------+         +------------------------+122.1.1.3|
       |122.1.1.1|eth0/5         eth0/4|         |                 eth0/6 |         |
       + --------+                     |         |                        |         |
                                       +----+----+                        +---------+
                                            |eth0/3
                                            |
                                            |
                                            |
                                            |10.1.1.4/24
                                            |eth0/8
                                      +----++------+
                                      |            |
                                      |   H2       |
                                      |122.1.1.2   |
                                      |            |
                                      +------------+
  */
  graph_t* t = create_new_graph((char* )"Simple L2 Switch graph");
  node_t* H1 = create_graph_node(t, (char* )"H1");
  node_t* H2 = create_graph_node(t, (char* )"H2");
  node_t* H3 = create_graph_node(t, (char* )"H3");
  node_t* H4 = create_graph_node(t, (char* )"H4");
  node_t* L2SW = create_graph_node(t, (char* )"L2SW");

  insert_link_between_nodes(H1, L2SW, (char *)"eth0/5", (char *)"eth0/4", 1);
  insert_link_between_nodes(H2, L2SW, (char *)"eth0/8", (char *)"eth0/3", 1);
  insert_link_between_nodes(H3, L2SW, (char *)"eth0/6", (char *)"eth0/2", 1);
  insert_link_between_nodes(H4, L2SW, (char *)"eth0/7", (char *)"eth0/1", 1);

  node_set_loopback_addr(H1, new ip_addr_t(122,1,1,1), 32);
  node_set_intf_ip_addr(H1, (char *)"eth0/5", new ip_addr_t(10,1,1,2), 24);

  node_set_loopback_addr(H2, new ip_addr_t(122,1,1,2), 32);
  node_set_intf_ip_addr(H2, (char *)"eth0/8", new ip_addr_t(10,1,1,4), 24);

  node_set_loopback_addr(H3, new ip_addr_t(122,1,1,3), 32);
  node_set_intf_ip_addr(H3, (char *)"eth0/6", new ip_addr_t(10,1,1,1), 24);

  node_set_loopback_addr(H4, new ip_addr_t(122,1,1,4), 32);
  node_set_intf_ip_addr(H4, (char *)"eth0/7", new ip_addr_t(10,1,1,3), 24);

  get_node_intf_by_name(L2SW, (char*)"eth0/1")->intf_nw_props.set_l2_mode(intf_l2_mode_t::ACCESS);
  get_node_intf_by_name(L2SW, (char*)"eth0/2")->intf_nw_props.set_l2_mode(intf_l2_mode_t::ACCESS);
  get_node_intf_by_name(L2SW, (char*)"eth0/3")->intf_nw_props.set_l2_mode(intf_l2_mode_t::ACCESS);
  get_node_intf_by_name(L2SW, (char*)"eth0/4")->intf_nw_props.set_l2_mode(intf_l2_mode_t::ACCESS);

  return t;
}

graph_t* l2_dualswitch_topo () {

/*
                                    +---------+                               +----------+
                                    |         |                               |          |
                                    |  H2     |                               |  H5      |
                                    |122.1.1.2|                               |122.1.1.5 |
                                    +---+-----+                               +-----+----+
                                        |10.1.1.2/24                                +10.1.1.5/24
                                        |eth0/3                                     |eth0/8
                                        |                                           |
                                        |eth0/7,AC,V10                              |eth0/9,AC,V10
                                  +-----+----+                                +-----+---+
                                  |          |                                |         |
   +------+---+                   |          |                                |         |                         +--------+
   |  H1      |10.1.1.1/24        |   L2SW1  |eth0/5                    eth0/7| L2SW2   |eth0/10           eth0/11|  H6    |
   |122.1.1.1 +-------------------|          |+-------------------------------|         +-------------+----------+122.1.1.6|
   +------+---+ eth0/1      eth0/2|          |TR,V10,V11            TR,V10,V11|         |AC,V10        10.1.1.6/24|        |
                            AC,V10|          |                                |         |                         +-+------+
                                  +-----+----+                                +----+----+
                                        |eth0/6                                    |eth0/12
                                        |AC,V11                                    |AC,V11
                                        |                                          |
                                        |                                          |
                                        |                                          |
                                        |                                          |eth0/11
                                        |eth0/4                                    |10.1.1.4/24
                                        |10.1.1.3/24                             +--+-----+
                                   +----+---+|                                   | H4     |
                                   |  H3     |                                   |        |
                                   |122.1.1.3|                                   |122.1.1.4|
                                   +--------+|                                   +--------+
*/

    graph_t* topo = create_new_graph((char* )"Dual L2 Switch graph");
    node_t* H1 = create_graph_node(topo, (char* )"H1");
    node_set_loopback_addr(H1, new ip_addr_t(122,1,1,1), 32);
    node_t* H2 = create_graph_node(topo, (char* )"H2");
    node_set_loopback_addr(H2, new ip_addr_t(122,1,1,2), 32);
    node_t* H3 = create_graph_node(topo, (char* )"H3");
    node_set_loopback_addr(H3, new ip_addr_t(122,1,1,3), 32);
    node_t* H4 = create_graph_node(topo, (char* )"H4");
    node_set_loopback_addr(H4, new ip_addr_t(122,1,1,4), 32);
    node_t* H5 = create_graph_node(topo, (char* )"H5");
    node_set_loopback_addr(H5, new ip_addr_t(122,1,1,5), 32);
    node_t* H6 = create_graph_node(topo, (char* )"H6");
    node_set_loopback_addr(H6, new ip_addr_t(122,1,1,6), 32);
    node_t* L2SW1 = create_graph_node(topo, (char* )"L2SW1");
    node_t* L2SW2 = create_graph_node(topo, (char* )"L2SW2");

    insert_link_between_nodes(H1, L2SW1, (char* )"eth0/1", (char* )"eth0/2", 1);
    insert_link_between_nodes(H2, L2SW1, (char* )"eth0/3", (char* )"eth0/7", 1);
    insert_link_between_nodes(H3, L2SW1, (char* )"eth0/4", (char* )"eth0/6", 1);
    insert_link_between_nodes(L2SW1, L2SW2, (char* )"eth0/5", (char* )"eth0/7", 1);
    insert_link_between_nodes(H5, L2SW2, (char* )"eth0/8", (char* )"eth0/9", 1);
    insert_link_between_nodes(H4, L2SW2, (char* )"eth0/11", (char* )"eth0/12", 1);
    insert_link_between_nodes(H6, L2SW2, (char* )"eth0/11", (char* )"eth0/10", 1);

    node_set_intf_ip_addr(H1, (char* )"eth0/1",  new ip_addr_t(10,1,1,1), 24);
    node_set_intf_ip_addr(H2, (char* )"eth0/3",  new ip_addr_t(10,1,1,2), 24);
    node_set_intf_ip_addr(H3, (char* )"eth0/4",  new ip_addr_t(10,1,1,3), 24);
    node_set_intf_ip_addr(H4, (char* )"eth0/11", new ip_addr_t(10,1,1,4), 24);
    node_set_intf_ip_addr(H5, (char* )"eth0/8",  new ip_addr_t(10,1,1,5), 24);
    node_set_intf_ip_addr(H6, (char* )"eth0/11", new ip_addr_t(10,1,1,6), 24);

    get_node_intf_by_name(L2SW1, (char* )"eth0/2")->intf_nw_props.set_l2_mode(intf_l2_mode_t::ACCESS);
    get_node_intf_by_name(L2SW1, (char* )"eth0/2")->intf_nw_props.set_vlan_membership(10);
    get_node_intf_by_name(L2SW1, (char* )"eth0/7")->intf_nw_props.set_l2_mode(intf_l2_mode_t::ACCESS);
    get_node_intf_by_name(L2SW1, (char* )"eth0/7")->intf_nw_props.set_vlan_membership(10);

    get_node_intf_by_name(L2SW1, (char* )"eth0/5")->intf_nw_props.set_l2_mode(intf_l2_mode_t::TRUNK);
    get_node_intf_by_name(L2SW1, (char* )"eth0/5")->intf_nw_props.set_vlan_membership(10);
    get_node_intf_by_name(L2SW1, (char* )"eth0/5")->intf_nw_props.set_vlan_membership(11);

    get_node_intf_by_name(L2SW1, (char* )"eth0/6")->intf_nw_props.set_l2_mode(intf_l2_mode_t::ACCESS);
    get_node_intf_by_name(L2SW1, (char* )"eth0/6")->intf_nw_props.set_vlan_membership(11);

    get_node_intf_by_name(L2SW2, (char* )"eth0/7")->intf_nw_props.set_l2_mode(intf_l2_mode_t::TRUNK);
    get_node_intf_by_name(L2SW2, (char* )"eth0/7")->intf_nw_props.set_vlan_membership(10);
    get_node_intf_by_name(L2SW2, (char* )"eth0/7")->intf_nw_props.set_vlan_membership(11);
    get_node_intf_by_name(L2SW2, (char* )"eth0/9")->intf_nw_props.set_l2_mode(intf_l2_mode_t::ACCESS);
    get_node_intf_by_name(L2SW2, (char* )"eth0/9")->intf_nw_props.set_vlan_membership(10);
    get_node_intf_by_name(L2SW2, (char* )"eth0/10")->intf_nw_props.set_l2_mode(intf_l2_mode_t::ACCESS);
    get_node_intf_by_name(L2SW2, (char* )"eth0/10")->intf_nw_props.set_vlan_membership(10);
    get_node_intf_by_name(L2SW2, (char* )"eth0/12")->intf_nw_props.set_l2_mode(intf_l2_mode_t::ACCESS);
    get_node_intf_by_name(L2SW2, (char* )"eth0/12")->intf_nw_props.set_vlan_membership(11);

    return topo;
}


#endif
