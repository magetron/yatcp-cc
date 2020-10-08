#ifndef LAYER2_CC
#define LAYER2_CC

#include "layer2.hh"

extern cli_def *cli;

// METHOD IMPL
void l2_frame_recv (node_t* node, interface_t* intf, uint8_t* pkt, uint32_t pkt_size) {
  if (DEBUG) {
    cli_print(cli, "l2_frame_recv(), node: %s, intf: %s", node->node_name, intf->intf_name);
    dump_pkt(pkt, pkt_size);
  }
  auto eth_hdr = reinterpret_cast<ethernet_hdr_t *>(pkt);
  if (!l2_frame_recv_qualify_on_intf(intf, eth_hdr)) {
    cli_print(cli, "interface %s of node %s rejects pkt on L2 Frame", intf->intf_name, node->node_name);
    return;
  } else {
    if (IS_INTF_L3_MODE(intf)) {
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
    } else if (intf->intf_nw_props.intf_l2_mode == intf_l2_mode_t::ACCESS ||
               intf->intf_nw_props.intf_l2_mode == intf_l2_mode_t::TRUNK ) {
      if (intf->intf_nw_props.intf_l2_mode == intf_l2_mode_t::ACCESS &&
          !get_vlan_hdr(eth_hdr) && intf->intf_nw_props.vlans[0] != 0) {
            pkt = reinterpret_cast<uint8_t*>(
              tag_pkt_with_vlan_id(eth_hdr, pkt_size, intf->intf_nw_props.vlans[0], &pkt_size);
            );
          }
      l2_switch_recv(node, intf, pkt, pkt_size);
    }
  }
}

void l2_switch_recv(node_t* node, interface_t* intf, uint8_t* pkt, uint32_t pkt_size) {
  auto* eth_hdr = reinterpret_cast<ethernet_hdr_t *>(pkt);
  l2_switch_mac_learning(node, &eth_hdr->src_addr, intf->intf_name);
  l2_switch_forward_frame(node, intf, pkt, pkt_size);
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

vlan_eth_hdr_t* tag_pkt_with_vlan_id (ethernet_hdr_t* eth_hdr,
                                      uint32_t pkt_size,
                                      uint32_t vlan_id,
                                      uint32_t* new_pkt_size) {
  vlan_eth_hdr_t* vlan_hdr = reinterpret_cast<vlan_eth_hdr_t*>(
                               malloc(pkt_size + sizeof(vlan_8021q_hdr_t)));
  vlan_hdr->dst_addr = eth_hdr->dst_addr;
  vlan_hdr->src_addr = eth_hdr->src_addr;
  vlan_hdr->ethertype = eth_hdr->ethertype;
  memcpy(vlan_hdr->payload, eth_hdr->payload, pkt_size - ETH_HDR_SIZE_EXCL_PAYLOAD);
  vlan_hdr->vlan_hdr.pcp = 0;
  vlan_hdr->vlan_hdr.dei = 0;
  vlan_hdr->vlan_hdr.vid = vlan_id;
  *new_pkt_size = pkt_size + sizeof(vlan_8021q_hdr_t);
  free(eth_hdr);
  return vlan_hdr;
}

ethernet_hdr_t* untag_pkt_with_vlan_id (vlan_eth_hdr_t* vlan_hdr,
                                        uint32_t pkt_size,
                                        uint32_t* new_pkt_size) {
  ethernet_hdr_t* eth_hdr = reinterpret_cast<ethernet_hdr_t*>(
                              malloc(pkt_size - sizeof(vlan_8021q_hdr_t)));
  eth_hdr->dst_addr = vlan_hdr->dst_addr;
  eth_hdr->src_addr = vlan_hdr->src_addr;
  eth_hdr->ethertype = vlan_hdr->ethertype;
  memcpy(eth_hdr->payload, vlan_hdr->payload, pkt_size - VLAN_ETH_HDR_SIZE_EXCL_PAYLOAD);
  *new_pkt_size = pkt_size - sizeof(vlan_8021q_hdr_t);
  free(vlan_hdr);
  return eth_hdr;
}

// DEBUG IMPL
void dump_arp_table (arp_table_t* arp_table) {
  assert(arp_table);

  cli_print(cli, "ARP table size = %zu", (*arp_table).map.size());
  for (auto& p : (*arp_table).map)
    cli_print(cli, "IP Addr = %u.%u.%u.%u,  MAC : %02X.%02X.%02X.%02X.%02X.%02X, INTF = %s",
        p.first.addr[0], p.first.addr[1],
        p.first.addr[2], p.first.addr[3],
        p.second.mac_addr.addr[0], p.second.mac_addr.addr[1],
        p.second.mac_addr.addr[2], p.second.mac_addr.addr[3],
        p.second.mac_addr.addr[4], p.second.mac_addr.addr[5],
        p.second.o_intf_name);
}

void dump_mac_table (mac_table_t* mac_table) {
  assert(mac_table);

  cli_print(cli, "MAC table size = %zu", mac_table->entries.size());
  for (auto& entry : mac_table->entries)
    cli_print(cli, "MAC : %02X.%02X.%02X.%02X.%02X.%02X, INTF = %s",
      entry.mac_addr.addr[0], entry.mac_addr.addr[1],
      entry.mac_addr.addr[2], entry.mac_addr.addr[3],
      entry.mac_addr.addr[4], entry.mac_addr.addr[5],
      entry.o_intf_name);
}

#endif
