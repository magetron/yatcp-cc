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

static bool l2_switch_send_pkt (uint8_t* pkt, uint32_t pkt_size, interface_t* o_intf) {
  if (DEBUG) {
    cli_print(cli, "l2 switch sending pkt on outbound interface %s", o_intf->intf_name);
  }
  assert(!IS_INTF_L3_MODE(o_intf));
  if (o_intf->intf_nw_props.intf_l2_mode == intf_l2_mode_t::L2_MODE_UNKNOWN) {
    return false;
  }

  ethernet_hdr_t* eth_hdr = reinterpret_cast<ethernet_hdr_t*>(pkt);
  vlan_8021q_hdr_t* vlan = get_vlan_hdr(eth_hdr);
  assert(vlan);

  switch (o_intf->intf_nw_props.intf_l2_mode) {
    case intf_l2_mode_t::ACCESS:
      if (o_intf->intf_nw_props.vlans[0] !=0 && vlan &&
          o_intf->intf_nw_props.vlans[0] == vlan->getVlanID()) {
        uint32_t new_pkt_size = 0;
        auto new_hdr = untag_pkt_with_vlan_id(reinterpret_cast<vlan_eth_hdr_t*>(eth_hdr), pkt_size, &new_pkt_size);
        return send_pkt(reinterpret_cast<uint8_t*>(new_hdr), new_pkt_size, o_intf);
      } else {
        return false;
      }
      break;
    case intf_l2_mode_t::TRUNK:
      if (vlan && o_intf->intf_nw_props.is_vlan_enabled_in_trunk(vlan->getVlanID())) {
        return send_pkt(pkt, pkt_size, o_intf);
      } else {
        return false;
      }
    default:
      return false;
  }
}

static bool l2_switch_send_pkt_flood (node_t* node, interface_t* recv_intf, uint8_t* pkt, uint32_t pkt_size) {
  for (uint32_t i = 0; i < MAX_INTF_PER_NODE; i++) {
    auto o_intf = node->intfs[i];
    if (!o_intf) break;
    if (o_intf != recv_intf) {
      l2_switch_send_pkt(pkt, pkt_size, o_intf);
    }
  }
  return true;
}

static bool l2_switch_forward_frame (node_t* node, interface_t* recv_intf, uint8_t* pkt, uint32_t pkt_size) {
  auto* eth_hdr = reinterpret_cast<ethernet_hdr_t *>(pkt);
  if (is_mac_broadcast_addr(&eth_hdr->dst_addr)) {
    return l2_switch_send_pkt_flood(node, recv_intf, pkt, pkt_size);
  } else {
    auto* entry = node->node_nw_props.mac_table->lookup(&eth_hdr->dst_addr);
    if (!entry) {
      return l2_switch_send_pkt_flood(node, recv_intf, pkt, pkt_size);
    } else {
      char* o_intf_name = entry->o_intf_name;
      interface_t* o_intf = get_node_intf_by_name(node, o_intf_name);
      if (!o_intf) {
        return false;
      } else {
        return l2_switch_send_pkt(pkt, pkt_size, o_intf);
      }
    }
  }
}

#endif