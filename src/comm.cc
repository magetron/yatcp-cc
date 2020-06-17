#ifndef COMM_CC
#define COMM_CC

#include "comm.hh"

extern cli_def* cli;

static unsigned short udp_port_number = 10000;

static unsigned short get_next_udp_port_number () {
	if (udp_port_number == 0) {
		cli_print(cli, "ERROR : No UDP port left"); 
		return 0;
	} else return udp_port_number++;
}

void init_udp_socket (node_t *node) {
	node -> udp_port_number = get_next_udp_port_number();

	int udp_sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	
	sockaddr_in node_addr;
	node_addr.sin_family = AF_INET;
	node_addr.sin_port = node -> udp_port_number;
	node_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(udp_sock_fd, (sockaddr *)&node_addr, sizeof(sockaddr)) == -1) {
		cli_print(cli, "ERROR : socket bind failed for Node %s", node -> node_name);
		return;
	}

	node -> udp_sock_fd = udp_sock_fd;
}


#endif
