#ifndef NET_HH
#define NET_HH

#define IP_ADDR_LENGTH 4
#define MAC_ADDR_LENGTH 6

#include <cstring>
#include <cassert>

struct ip_addr_t;
struct mac_addr_t;
struct node_nw_props_t;
struct intf_nw_props_t;

struct ip_addr_t {
	unsigned short addr[IP_ADDR_LENGTH];
};
	
struct mac_addr_t {
	unsigned short addr[MAC_ADDR_LENGTH];
};

struct node_nw_props_t {

	// L3 properties
	bool has_lb_addr_config;
	ip_addr_t lb_addr;

	node_nw_props_t() : has_lb_addr_config(true) {
		lb_addr.addr[0] = 127;
		lb_addr.addr[1] = 0;
		lb_addr.addr[2] = 0;
		lb_addr.addr[3] = 1;
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

// GRAPH IMPORTS FOR METHODS DECL
struct graph_t;
struct node_t;
struct interface_t;

// METHODS
bool node_set_loopback_addr (node_t *node, ip_addr_t* ip_addr);

bool node_set_intf_ip_addr (node_t* node, char *local_intf, ip_addr_t *ip_addr, unsigned char mask);

bool node_unset_intf_ip_addr (node_t *node, char *local_intf);


#endif
