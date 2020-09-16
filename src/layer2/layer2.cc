#ifndef LAYER2_CC
#define LAYER2_CC

#include "layer2.hh"
#include "../comm.hh"

extern cli_def *cli;

// METHOD IMPL
void l2_frame_recv (node_t *node, interface_t *intf, unsigned char *pkt, unsigned int pkt_size) {
  // TODO Ingress
}

void send_arp_broadcast_request (node_t *node, interface_t *o_intf, ip_addr_t *ip) {
  cli_print(cli, "Sending ARP broadcast request ... node name = %s, ip_addr = %u.%u.%u.%u", node -> node_name, /*o_intf -> intf_name,*/ ip -> addr[0], ip -> addr[1], ip -> addr[2], ip -> addr[3]);
  void *arp_broadcast = malloc(ETH_HDR_SIZE_EXCL_PAYLOAD + sizeof(arp_hdr_t));
  ethernet_hdr_t *ethernet_hdr = reinterpret_cast<ethernet_hdr_t *>(arp_broadcast);
  if (!o_intf) { // outbound interface not sepcified
    o_intf = node_get_matching_subnet_intf(node, ip);
    if (!o_intf) { // if no such node found
      cli_print(cli, "ERROR : %s : No eligible subnet for ARP resolution for IP %u.%u.%u.%u",
                node -> node_name, ip -> addr[0], ip -> addr[1], ip -> addr[2], ip -> addr[3]);
      return;
    }
  }
  layer2_fill_with_broadcast_mac(&ethernet_hdr->dst_addr);
  memcpy(ethernet_hdr->src_addr.addr, INTF_MAC(o_intf), sizeof(mac_addr_t));
  ethernet_hdr->ethertype = ethernet_hdr_t::ARP_TYPE;

  arp_hdr_t *arp_hdr = new (ethernet_hdr->payload) arp_hdr_t();
  arp_hdr->h_type = 1;
  arp_hdr->p_type = 0x0800;
  arp_hdr->h_len  = sizeof(mac_addr_t);
  arp_hdr->p_len  = 4;
  arp_hdr->oper   = arp_hdr_t::ARP_BROADCAST_REQ;
  memcpy(arp_hdr->src_mac.addr, INTF_MAC(o_intf), sizeof(mac_addr_t));
  inet_pton(AF_INET, reinterpret_cast<char *>(&INTF_IP(o_intf)), &arp_hdr->src_ip);
  arp_hdr->src_ip.htonl();
  memset(arp_hdr->dst_mac.addr, 0, sizeof(mac_addr_t));
  inet_pton(AF_INET, reinterpret_cast<char *>(ip->addr), &arp_hdr->dst_ip);
  arp_hdr->dst_ip.htonl();
  uint32_t *fcs = reinterpret_cast<uint32_t *>(arp_hdr + 1); fcs = 0;

  send_pkt(reinterpret_cast<char *>(arp_broadcast), ETH_HDR_SIZE_EXCL_PAYLOAD + sizeof(arp_hdr_t), o_intf);

  free(arp_broadcast);
}

void process_arp_broadcast_req (node_t *node, interface_t *i_intf, ethernet_hdr_t *eth_hdr) {

}

void send_arp_reply_msg (ethernet_hdr_t *eth_hdr_in, interface_t *o_intf) {

}

void process_arp_reply_msg (node_t *node, interface_t *i_intf, ethernet_hdr_t *eth_hdr) {

}

// DEBUG IMPL
void dump_arp_table (arp_table_t *arp_table) {
  assert(arp_table);

  // TEST CODE TO BE REMOVED
  // BEGIN TEST
  ip_addr_t *ip = new ip_addr_t(1,2,2,1);
  mac_addr_t *mac = new mac_addr_t(16, 25, 98, 66, 64, 120);
  char *intf_name = (char *)malloc(sizeof(char) * INTF_NAME_SIZE);
  memset(intf_name, 0, INTF_NAME_SIZE);
  memcpy(intf_name, "RAND_INTF", 9);
  arp_table -> add(ip, mac, intf_name);
  // END TEST

  cli_print(cli, "ARP table size = %zu", (*arp_table).map.size());
  for (auto& p : (*arp_table).map)
    cli_print(cli, "IP : %u.%u.%u.%u,  MAC : %02X.%02X.%02X.%02X.%02X.%02X, O_INTF = %s\n",
        p.first.addr[0], p.first.addr[1], p.first.addr[2], p.first.addr[3],
        p.second.mac_addr.addr[0], p.second.mac_addr.addr[1], p.second.mac_addr.addr[2], p.second.mac_addr.addr[3], p.second.mac_addr.addr[4], p.second.mac_addr.addr[5],
        p.second.o_intf_name);
}


#endif
