#ifndef NET_HH
#define NET_HH

#include "config.hh"
#include "import.hh"

struct ip_addr_t;
struct mac_addr_t;
struct node_nw_props_t;
struct intf_nw_props_t;

struct ip_addr_t {
	unsigned char addr[IP_ADDR_LENGTH];

	ip_addr_t () { }

	ip_addr_t (unsigned char a, unsigned char b, unsigned char c, unsigned char d) {
		addr[0] = a;
		addr[1] = b;
		addr[2] = c;
		addr[3] = d;
	}

	bool operator == (const ip_addr_t& ip) {
		for (unsigned int i = 0; i < IP_ADDR_LENGTH; i++) 
			if (addr[i] != ip.addr[i]) return false;
		return true;
	}

	friend bool operator == (const ip_addr_t& ip1, const ip_addr_t& ip2) {
		for (unsigned int i = 0; i < IP_ADDR_LENGTH; i++)
			if (ip1.addr[i] != ip2.addr[i]) return false;
		return true;
	}

	~ip_addr_t () { }

};

static inline ip_addr_t *string_to_ip_addr_t (char *str) {
	unsigned short n = 0;
	unsigned char sec = 0;
	ip_addr_t *ip = new ip_addr_t();
	do {
		if ((*str < '0' || *str > '9') &&  (*str != '.')) {
			delete ip;
			return nullptr;
		} else if (*str >= '0' && *str <= '9') {
			n = n * 10 + (*str - '0');
			if (n > 255) {
				delete ip;
				return nullptr;
			}
		} else {
			ip -> addr[sec] = n;
			n = 0;
			sec++;
		}
		str++;
	} while (*str != '\0');
	ip -> addr[sec] = n;
	return ip;
}

namespace std {
	template <>
		struct hash<ip_addr_t> {
			size_t operator () (const ip_addr_t& ip) const {
				return hash<int>()((ip.addr[0] << 12) + (ip.addr[1] << 8) + (ip.addr[2] << 4) + ip.addr[3]);
			}
		};
}
	
struct mac_addr_t {
	unsigned char addr[MAC_ADDR_LENGTH];

	mac_addr_t () { }

	mac_addr_t (unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f) {
		addr[0] = a;
		addr[1] = b;
		addr[2] = c;
		addr[3] = d;
		addr[4] = e;
		addr[5] = f;
	}

	bool operator == (const mac_addr_t& mac) {
		for (unsigned int i = 0; i < MAC_ADDR_LENGTH; i++) 
			if (addr[i] != mac.addr[i]) return false;
		return true;
	}

	~mac_addr_t () { }

};

// TODO : LOL, such hacky
#include "layer2/arp.hh"

struct node_nw_props_t {

	// L2 properties
	arp_table_t *arp_table;

	// L3 properties
	bool has_lb_addr_config;
	ip_addr_t lb_addr;
	unsigned char mask;

	// Send buffer
	unsigned char *send_buffer;

	node_nw_props_t () : has_lb_addr_config(true), mask(32) {
		lb_addr.addr[0] = 127;
		lb_addr.addr[1] = 0;
		lb_addr.addr[2] = 0;
		lb_addr.addr[3] = 1;
		send_buffer = (unsigned char *)malloc(sizeof(unsigned char) * (MAX_AUX_INFO_SIZE + MAX_PKT_BUFFER_SIZE));
		arp_table = new arp_table_t();
	}

	~node_nw_props_t () {
		free(send_buffer);
		delete arp_table;
	}

};

struct intf_nw_props_t {
	
	// L2 properties
	mac_addr_t mac_addr;

	// L3 properties
	bool has_ip_addr_config;
	ip_addr_t ip_addr;
	unsigned char mask;

	intf_nw_props_t() : has_ip_addr_config(false), mask(0) {
		memset(&(mac_addr), 0, sizeof(mac_addr_t));
		memset(&(ip_addr.addr), 0, sizeof(ip_addr_t));
	}

};

// HELPER
#define INTF_MAC(intf_ptr)		((intf_ptr) -> intf_nw_props.mac_addr.addr)
#define INTF_IP(intf_ptr)		((intf_ptr) -> intf_nw_props.ip_addr.addr)
#define NODE_LB_ADDR(node_ptr)	((node_ptr) -> node_nw_props.lb_addr.addr)
#define IS_INTF_L3_MODE(intf_ptr) ((intf_ptr) -> intf_nw_props.has_ip_addr_config)

// GRAPH IMPORTS FOR METHODS DECL
struct graph_t;
struct node_t;
struct interface_t;

// METHODS
bool node_set_intf_ip_addr (node_t *node, ip_addr_t* ip_addr);

bool node_set_loopback_addr (node_t *node, ip_addr_t* ip_addr, unsigned char mask);

bool node_set_intf_ip_addr (node_t* node, char *local_intf, ip_addr_t *ip_addr, unsigned char mask);

bool node_unset_intf_ip_addr (node_t *node, char *local_intf);

void intf_assign_mac_addr (interface_t *intf);

interface_t *node_get_matching_subnet_intf (node_t *node, ip_addr_t *ip_addr);

unsigned char *pkt_buffer_shift_right (unsigned char *pkt, unsigned int pkt_size, unsigned int buffer_size);

// DEBUG

void dump_node_nw_props (node_t *node);

void dump_intf_props (interface_t *intf);

void dump_nw_graph (graph_t *graph);

#endif
