#ifndef COMM_HH
#define COMM_HH

#include "graph.hh"

void init_udp_socket (node_t *node);

void nw_start_pkt_receiver_thread (graph_t *topo);

int send_pkt (char *pkt, unsigned short pkt_size, interface_t *intf);

int send_pkt_flood (node_t *node, interface_t *exempted_intf, char *pkt, unsigned short pkt_size);

#endif
