#ifndef COMM_HH
#define COMM_HH

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "graph.hh"
#include "nwcli.hh"

void init_udp_socket (node_t *node);

#endif
