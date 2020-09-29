#ifndef LAYER2_CC
#define LAYER2_CC

#include "layer2.hh"
#include "../comm.hh"

extern cli_def *cli;

// METHOD IMPL
void l2_frame_recv (node_t* node, interface_t* intf, uint8_t* pkt, uint32_t pkt_size) {
  auto eth_hdr = reinterpret_cast<ethernet_hdr_t *>(pkt);
  if (!l2_frame_recv_qualify_on_intf(intf, eth_hdr)) {
    cli_print(cli, "interface %s of node %s rejects pkt on L2 Frame", intf->intf_name, node->node_name);
    return;
  } else {
    switch (eth_hdr->ethertype) {
      case ethernet_hdr_t::ARP_TYPE:
      {
        auto arp_hdr = reinterpret_cast<arp_hdr_t *>(eth_hdr->payload);
        switch (arp_hdr->oper) {
          case arp_hdr_t::ARP_BROADCAST_REQ:
            process_arp_broadcast_req(node, intf, eth_hdr);
            break;
          case arp_hdr_t::ARP_REPLY:
            process_arp_reply_msg(node, intf, eth_hdr);
            break;
        }
      }
      break;
      default:
        promote_pkt_to_l3(node, intf, pkt, pkt_size);
    }
  }
}

// TODO: impl on L3 parsing complete
void promote_pkt_to_l3 (node_t* node, interface_t* intf, uint8_t* pkt, uint32_t pkt_size) {
  cli_print(cli, "pkt promoted to L3");
}

void send_arp_broadcast_request (node_t* node, interface_t* o_intf, ip_addr_t* ip) {
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
  arp_hdr->src_ip = o_intf->intf_nw_props.ip_addr;
  arp_hdr->src_ip.htonl();
  memset(arp_hdr->dst_mac.addr, 0, sizeof(mac_addr_t));
  arp_hdr->dst_ip = *ip;
  arp_hdr->dst_ip.htonl();
  uint32_t *fcs = reinterpret_cast<uint32_t *>(arp_hdr + 1); fcs = 0;

  send_pkt(reinterpret_cast<uint8_t *>(arp_broadcast), ETH_HDR_SIZE_EXCL_PAYLOAD + sizeof(arp_hdr_t), o_intf);

  free(arp_broadcast);
}

void process_arp_broadcast_req (node_t* node, interface_t* i_intf, ethernet_hdr_t* eth_hdr) {
  cli_print(cli, "ARP Broadcast msg received on intf %s of node %s",
            i_intf->intf_name, i_intf->att_node->node_name);
  arp_hdr_t *arp_hdr = reinterpret_cast<arp_hdr_t *>(eth_hdr->payload);
  arp_hdr->dst_ip.htonl();
  if (arp_hdr->dst_ip == i_intf->intf_nw_props.ip_addr) {
    send_arp_reply_msg(eth_hdr, i_intf);
  }
}

void send_arp_reply_msg (ethernet_hdr_t* eth_hdr_in, interface_t* o_intf) {
  cli_print(cli, "Sending ARP reply msg ... intf name = %s, ip_addr = %u.%u.%u.%u",
      o_intf->intf_name, INTF_IP(o_intf)[0], INTF_IP(o_intf)[1], INTF_IP(o_intf)[2], INTF_IP(o_intf)[3]);
  arp_hdr_t *arp_hdr_in = reinterpret_cast<arp_hdr_t *>(eth_hdr_in->payload);
  void *arp_reply = malloc(ETH_HDR_SIZE_EXCL_PAYLOAD + sizeof(arp_hdr_t));
  ethernet_hdr_t* eth_hdr = reinterpret_cast<ethernet_hdr_t *>(arp_reply);
  eth_hdr->dst_addr = arp_hdr_in->src_mac;
  eth_hdr->src_addr = o_intf->intf_nw_props.mac_addr;
  eth_hdr->ethertype = ethernet_hdr_t::ARP_TYPE;
  arp_hdr_t* arp_hdr = reinterpret_cast<arp_hdr_t *>(eth_hdr->payload);
  arp_hdr->h_type = 1;
  arp_hdr->p_type = 0x0800;
  arp_hdr->h_len = sizeof(mac_addr_t);
  arp_hdr->p_len = 4;
  arp_hdr->oper  = arp_hdr_t::ARP_REPLY;
  arp_hdr->src_mac = o_intf->intf_nw_props.mac_addr;
  arp_hdr->src_ip = o_intf->intf_nw_props.ip_addr;
  arp_hdr->src_ip.htonl();
  arp_hdr->dst_mac = arp_hdr_in->src_mac;
  arp_hdr->dst_ip = arp_hdr_in->src_ip;
  arp_hdr->dst_ip.htonl();
  uint32_t *fcs = reinterpret_cast<uint32_t *>(arp_hdr + 1); fcs = 0;
  send_pkt(reinterpret_cast<uint8_t *>(arp_reply), ETH_HDR_SIZE_EXCL_PAYLOAD + sizeof(arp_hdr_t), o_intf);
  free(arp_reply);
}

void process_arp_reply_msg (node_t* node, interface_t* i_intf, ethernet_hdr_t* eth_hdr) {
  cli_print(cli, "ARP reply msg received on interface %s of node %s",
      i_intf->intf_name, node->node_name);
  auto table = node->node_nw_props.arp_table;
  auto arp_hdr = reinterpret_cast<arp_hdr_t *>(eth_hdr->payload);
  arp_hdr->src_ip.htonl();
  if (table->lookup(&arp_hdr->src_ip)) {
    table->update(arp_hdr, i_intf->intf_name);
  } else {
    table->add(&arp_hdr->src_ip, &arp_hdr->src_mac, i_intf->intf_name);
  }
}

// DEBUG IMPL
void dump_arp_table (arp_table_t* arp_table) {
  assert(arp_table);

  cli_print(cli, "ARP table size = %zu", (*arp_table).map.size());
  for (auto& p : (*arp_table).map)
    cli_print(cli, "IP Addr = %u.%u.%u.%u,  MAC : %02X.%02X.%02X.%02X.%02X.%02X, INTF = %s",
        p.first.addr[0], p.first.addr[1], p.first.addr[2], p.first.addr[3],
        p.second.mac_addr.addr[0], p.second.mac_addr.addr[1], p.second.mac_addr.addr[2], p.second.mac_addr.addr[3], p.second.mac_addr.addr[4], p.second.mac_addr.addr[5],
        p.second.o_intf_name);
}

void dump_mac_table (mac_table_t* mac_table) {
  assert(mac_table);

  cli_print(cli, "MAC table size = %zu", mac_table->entries.size());
  for (auto& entry : mac_table->entries)
    cli_print(cli, "MAC : %02X.%02X.%02X.%02X.%02X.%02X",
      entry.mac_addr.addr[0], entry.mac_addr.addr[1],
      entry.mac_addr.addr[2], entry.mac_addr.addr[3],
      entry.mac_addr.addr[4], entry.mac_addr.addr[5]);
}


#endif
