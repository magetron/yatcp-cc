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

// TODO
static void _pkt_receive (node_t* node, char *pkt_with_aux_data, unsigned short bytes_recvd) {
	return;
}

static void _pkt_receiver_thread_func (void *arg) {
	fd_set active_sockets, backup_sockets;
	
	int socket_max_fd = -1;
	unsigned short bytes_recvd = -1;

	graph_t *topo = (graph_t *)arg;
	
	FD_ZERO(&active_sockets); FD_ZERO(&backup_sockets);

	for (auto& node : topo -> node_list ) if (node -> udp_sock_fd) {
		if (node -> udp_sock_fd > socket_max_fd) socket_max_fd = node -> udp_sock_fd;

		FD_SET(node -> udp_sock_fd, &backup_sockets);
	}
	
	sockaddr_in sender_addr;
	char *recv_buffer = (char *)malloc(sizeof(char) * MAX_PKT_BUFFER_SIZE);

	while (true) {
		memcpy(&active_sockets, &backup_sockets, sizeof(fd_set));
		select(socket_max_fd + 1, &active_sockets, nullptr, nullptr, nullptr);
		
		for (auto& node : topo -> node_list)
			if (FD_ISSET(node -> udp_sock_fd, &active_sockets)) {
				memset(recv_buffer, 0, MAX_PKT_BUFFER_SIZE);
				bytes_recvd = recvfrom(node -> udp_sock_fd, recvfrom, MAX_PKT_BUFFER_SIZE, 0, &sender_addr, sizeof(sockaddr));
				_pkt_receive(node, recv_buffer, bytes_recvd);
			}
	}
			
}


void nw_start_pkt_receiver_thread (graph_t *topo) {
	pthread_attr_t attr;
	pthread_t pkt_receiver_thread;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_create(&pkt_receiver_thread, &attr, _pkt_receiver_thread_func, (void *) topo);

}

#endif
