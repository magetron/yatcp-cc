#ifndef L2SWITCH_HH
#define L2SWITCH_HH

#include "l2config.hh"
#include "l2import.hh"

#include "../net.hh"
#include "../comm.hh"

static void l2_switch_mac_learning (node_t* node, mac_addr_t* src_mac, char* intf_name) {
  if (node->node_nw_props.mac_table->lookup(src_mac)) {
    node->node_nw_props.mac_table->update(src_mac, intf_name);
  } else {
    node->node_nw_props.mac_table->add(src_mac, intf_name);
  }
}

static void l2_switch_forward_frame (node_t* node, interface_t* recv_intf, uint8_t* pkt, uint32_t pkt_size) {
  auto* eth_hdr = reinterpret_cast<ethernet_hdr_t *>(pkt);
  if (is_mac_broadcast_addr(&eth_hdr->dst_addr)) {
    send_pkt_flood_l2_intf(node, recv_intf, pkt, pkt_size);
    return;
  } else {
    auto* entry = node->node_nw_props.mac_table->lookup(&eth_hdr->dst_addr);
    if (!entry) {
      send_pkt_flood_l2_intf(node, recv_intf, pkt, pkt_size);
    } else {
      char* o_intf_name = entry->o_intf_name;
      interface_t* o_intf = get_node_intf_by_name(node, o_intf_name);
      if (!o_intf) {
        return;
      } else {
        send_pkt(pkt, pkt_size, o_intf);
      }
    }
  }
}

#endif