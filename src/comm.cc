#ifndef COMM_CC
#define COMM_CC

#include "comm.hh"
#include "nwcli.hh"

extern cli_def* cli;

static unsigned short udp_port_number = 10000;
unsigned char recv_buffer[MAX_AUX_INFO_SIZE + MAX_PKT_BUFFER_SIZE];

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

// Entry point into data link layer from physical layer, ingress journey of packet starts from here in the TCP/IP stack
int pkt_receive (node_t *node, interface_t *intf, unsigned char *pkt, unsigned short pkt_size) {
	printf("msg received = '%s', on node = %s, ingress intf = %s\n", pkt, node -> node_name, intf -> intf_name);
	return 0;
}

static void _pkt_receive (node_t* node, unsigned char *pkt_with_aux_data, unsigned short bytes_recvd) {
	interface_t *recv_intf = get_node_intf_by_name(node, (char *)pkt_with_aux_data);

	if (!recv_intf) {
		cli_print(cli, "ERROR : packet received on interface %s cannot be found on node %s", (char *)pkt_with_aux_data, node -> node_name);
		return;
	}

	pkt_receive(node, recv_intf, pkt_with_aux_data + INTF_NAME_SIZE, bytes_recvd - INTF_NAME_SIZE);

}

static void *_pkt_receiver_thread_func (void *arg) {
	fd_set active_sockets, backup_sockets;
	
	int socket_max_fd = -1;

	graph_t *topo = (graph_t *)arg;
	
	FD_ZERO(&active_sockets); FD_ZERO(&backup_sockets);

	for (auto& node : topo -> node_list ) if (node -> udp_sock_fd) {
		if (node -> udp_sock_fd > socket_max_fd) socket_max_fd = node -> udp_sock_fd;

		FD_SET(node -> udp_sock_fd, &backup_sockets);
	}
	
	sockaddr_in sender_addr;
	socklen_t addr_len = sizeof(sockaddr);

	while (true) {
		memcpy(&active_sockets, &backup_sockets, sizeof(fd_set));
		select(socket_max_fd + 1, &active_sockets, nullptr, nullptr, nullptr);
		
		for (auto& node : topo -> node_list)
			if (FD_ISSET(node -> udp_sock_fd, &active_sockets)) {
				memset(recv_buffer, 0, MAX_PKT_BUFFER_SIZE);
				ssize_t bytes_recvd = recvfrom(node -> udp_sock_fd, (unsigned char *)recv_buffer, MAX_PKT_BUFFER_SIZE, 0, (sockaddr *)&sender_addr, (socklen_t *)&addr_len);
				_pkt_receive(node, (unsigned char *)recv_buffer, bytes_recvd);
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

static int _send_pkt (int sock_fd, unsigned char *pkt_data, unsigned short pkt_size, unsigned short recv_udp_port_no) {
	sockaddr_in recv_addr;
	hostent *host = (hostent *)gethostbyname("127.0.0.1");
	recv_addr.sin_family = AF_INET;
	recv_addr.sin_port = recv_udp_port_no;
	recv_addr.sin_addr = *((in_addr *)host -> h_addr);

	return sendto(sock_fd, pkt_data, pkt_size, 0, (sockaddr *)&recv_addr, sizeof(sockaddr));
}


int send_pkt (unsigned char *pkt, unsigned short pkt_size, interface_t *intf) {
	node_t *send_node = intf -> att_node;
	node_t *recv_node = get_nbr_node(intf);

	if (!recv_node) {
		cli_print(cli, "ERROR : no recv node connected to the interface");
		return -1;
	}
	
	if (pkt_size > MAX_PKT_BUFFER_SIZE) {
		cli_print(cli, "ERROR : packet size exceeds maximum limit");
		return -1;
	}

	unsigned short recv_udp_port_no = recv_node -> udp_port_number;

	int sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock_fd < 0) {
		cli_print(cli, "ERROR : sending socket creation failed");
		return -1;
	}

	interface_t *recv_intf = &(intf -> link -> intf1) == intf ? &(intf -> link -> intf2) : &(intf -> link -> intf1);
	
	unsigned char *send_buffer = send_node -> node_nw_props.send_buffer;

	memset(send_buffer, 0, MAX_AUX_INFO_SIZE + MAX_PKT_BUFFER_SIZE);
		
	strncpy((char *)send_buffer, recv_intf -> intf_name, INTF_NAME_SIZE);
	send_buffer[INTF_NAME_SIZE - 1] = '\0';
	memcpy(send_buffer + INTF_NAME_SIZE, pkt, pkt_size);
	
	int send_result = _send_pkt(sock_fd, send_buffer, pkt_size + INTF_NAME_SIZE, recv_udp_port_no);

	close(sock_fd);
	return send_result;
}

int send_pkt_flood (node_t *node, interface_t *exempted_intf, unsigned char *pkt, unsigned short pkt_size) {
	for (unsigned int i = 0; i < MAX_INTF_PER_NODE; i++) 
		if (!node -> intfs[i]) break;
		else if (node -> intfs[i] != exempted_intf) {
			int ret = send_pkt(pkt, pkt_size, node -> intfs[i]);
			if (ret == -1) return -1;
		}
	return 0;
}
	

#endif
