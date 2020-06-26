#ifndef CONFIG_HH
#define CONFIG_HH

// graph
#define NODE_NAME_SIZE 16
#define INTF_NAME_SIZE 16
#define MAX_INTF_PER_NODE 8
#define TOPO_NAME_SIZE 32

// net
#define IP_ADDR_LENGTH 4
#define MAC_ADDR_LENGTH 6

// comm
#define MAX_PKT_BUFFER_SIZE 2048
#define MAX_AUX_INFO_SIZE INTF_NAME_SIZE
#define UDP_PORT_START 10000

// nwcli
#define TELNET_PORT 10023

#endif
