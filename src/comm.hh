#ifndef COMM_HH
#define COMM_HH

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netdb.h>
#include <cstdlib>

#include "graph.hh"
#include "nwcli.hh"

#define MAX_PKT_BUFFER_SIZE 2048
#define MAX_AUX_INFO_SIZE INTF_NAME_SIZE 


void init_udp_socket (node_t *node);

void nw_start_pkt_receiver_thread (graph_t *topo);

int send_pkt (char *pkt, unsigned short pkt_size, interface_t *intf);

int send_pkt_flood (node_t *node, interface_t *exempted_intf, char *pkt, unsigned short pkt_size);

#endif
