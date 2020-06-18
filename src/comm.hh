#ifndef COMM_HH
#define COMM_HH

#define MAX_PKT_BUFFER_SIZE 2048

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <cstdlib>

#include "graph.hh"
#include "nwcli.hh"

void init_udp_socket (node_t *node);

void nw_start_pkt_receiver_thread (graph_t *topo);

#endif
