#ifndef LAYER2_HH
#define LAYER2_HH

#include "l2config.hh"
#include "l2import.hh"

#include "../net.hh"
#include "../utils.hh"
#include "../nwcli.hh"
#include "../comm.hh"
#include "../pktdump.hh"

#include "arp.hh"
#include "mac.hh"
#include "ethernet.hh"
#include "l2switch.hh"

// METHOD
void l2_frame_recv (node_t* node, interface_t* intf, uint8_t* pkt, uint32_t pkt_size);

void l2_switch_recv (node_t* node, interface_t* intf, uint8_t* pkt, uint32_t pkt_size);

void promote_pkt_to_l3 (node_t* node, interface_t* intf, uint8_t* pkt, uint32_t pkt_size);

void send_arp_broadcast_request (node_t* node, interface_t* o_intf, ip_addr_t* ip);

void process_arp_broadcast_req (node_t* node, interface_t* i_intf, ethernet_hdr_t* eth_hdr);

void send_arp_reply_msg (ethernet_hdr_t* eth_hdr_in, interface_t* o_intf);

void process_arp_reply_msg (node_t* node, interface_t* i_intf, ethernet_hdr_t* eth_hdr);

// DEBUG
void dump_arp_table (arp_table_t* arp_table);

void dump_mac_table (mac_table_t* mac_table);

#endif
