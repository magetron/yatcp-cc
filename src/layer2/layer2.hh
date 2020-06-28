#ifndef LAYER2_HH
#define LAYER2_HH

#include "l2config.hh"
#include "l2import.hh"

#include "../net.hh"
#include "../utils.hh"

#include "arp.hh"
#include "ethernet.hh"

void l2_frame_recv (node_t *node, interface_t *intf, unsigned char *pkt, unsigned int pkt_size);

void send_arp_broadcast_request (node_t *node, interface_t *o_intf, ip_addr_t *ip);

// DEBUG
void dump_arp_table (arp_table_t *arp_table);

#endif
