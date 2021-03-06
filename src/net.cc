#ifndef NET_CC
#define NET_CC

#include "net.hh"
#include "graph.hh"
#include "nwcli.hh"
#include "utils.hh"

extern cli_def *cli;

// METHODS IMPL
bool node_set_loopback_addr (node_t *node, ip_addr_t* ip_addr) {
  return node_set_loopback_addr(node, ip_addr, 32);
}

bool node_set_loopback_addr (node_t *node, ip_addr_t* ip_addr, unsigned char mask) {
  assert(ip_addr);
  node -> node_nw_props.has_lb_addr_config = true;
  node -> node_nw_props.mask = mask;
  memcpy(NODE_LB_ADDR(node), ip_addr, IP_ADDR_LENGTH);
  return true;
}

bool node_set_intf_ip_addr (node_t *node, char *local_intf, ip_addr_t *ip_addr, unsigned char mask) {
  interface_t *interface = get_node_intf_by_name(node, local_intf);
  if (!interface) assert(0);

  memcpy(INTF_IP(interface), ip_addr, IP_ADDR_LENGTH);
  interface -> intf_nw_props.mask = mask;
  interface -> intf_nw_props.has_ip_addr_config = true;
  return true;
}

bool node_unset_intf_ip_addr (node_t *node, char *local_intf) {
  interface_t *interface = get_node_intf_by_name(node, local_intf);
  if (!interface) assert(0);

  interface -> intf_nw_props.has_ip_addr_config = false;
  interface -> intf_nw_props.mask = 0;
  memset(INTF_IP(interface), 0, IP_ADDR_LENGTH);
  return true;
}

interface_t *node_get_matching_subnet_intf (node_t *node, ip_addr_t *ip_addr) {
  for (unsigned int i = 0; i < MAX_INTF_PER_NODE; i++)
    if (!node -> intfs[i]) return nullptr;
    else if (IS_INTF_L3_MODE(node -> intfs[i])) {
      auto intf_ip_masked = apply_mask(&node->intfs[i]->intf_nw_props.ip_addr, node->intfs[i]->intf_nw_props.mask);
      auto ip_addr_masked = apply_mask(ip_addr, node->intfs[i]->intf_nw_props.mask);
      if (intf_ip_masked == ip_addr_masked) {
        return node -> intfs[i];
      }
    }
  return nullptr;
}

// HASH MAC ADDR GENERATION - should assign according to manufacturer and serial number in real-life scenarios
void intf_assign_mac_addr (interface_t *intf) {
  node_t *node = intf -> att_node;

  if (!node) return;

  srand((unsigned long)intf);

  for (int i = 0; i < MAC_ADDR_LENGTH; i++)
    (intf) -> intf_nw_props.mac_addr.addr[i] = rand() % 256;
}

uint8_t* pkt_buffer_shift_right (uint8_t* pkt, unsigned int pkt_size, unsigned int buffer_size) {
  memcpy(pkt + (buffer_size - pkt_size), pkt, pkt_size);
  memset(pkt, 0, pkt_size);
  return pkt + (buffer_size - pkt_size);
}

// DEBUG IMPL
void dump_node_nw_props (node_t *node) {

    cli_print(cli, "\nNode Name = %s", node -> node_name);
    if(node -> node_nw_props.has_lb_addr_config)
    cli_print(cli, "\t loopback addr : %u.%u.%u.%u/%u", NODE_LB_ADDR(node)[0], NODE_LB_ADDR(node)[1], NODE_LB_ADDR(node)[2], NODE_LB_ADDR(node)[3], node -> node_nw_props.mask);
}

void dump_intf_props (interface_t *intf) {
  dump_interface(intf);

  if(intf->intf_nw_props.has_ip_addr_config) {
    cli_print(cli, "\t IP Addr = %u.%u.%u.%u/%u", INTF_IP(intf)[0], INTF_IP(intf)[1], INTF_IP(intf)[2], INTF_IP(intf)[3], intf -> intf_nw_props.mask);
  } else {
    cli_print(cli, "\t IP Addr = %s/%u", "null", 0);
  }

  cli_print(cli, "\t L2 mode : %s", intf_l2_mode_str(intf->intf_nw_props.intf_l2_mode).c_str());

  if (intf->intf_nw_props.intf_l2_mode != intf_l2_mode_t::L2_MODE_UNKNOWN &&
      !intf->intf_nw_props.has_ip_addr_config) {
    if (intf->intf_nw_props.intf_l2_mode == intf_l2_mode_t::ACCESS) {
      cli_print(cli, "\t VLAN ID = %u", intf->intf_nw_props.vlans[0]);
    } else {
      cli_print(cli, "\t VLAN IDs = %u, %u, %u, %u, %u, %u, %u, %u, %u, %u",
                     intf->intf_nw_props.vlans[0], intf->intf_nw_props.vlans[1],
                     intf->intf_nw_props.vlans[2], intf->intf_nw_props.vlans[3],
                     intf->intf_nw_props.vlans[4], intf->intf_nw_props.vlans[5],
                     intf->intf_nw_props.vlans[6], intf->intf_nw_props.vlans[7],
                     intf->intf_nw_props.vlans[8], intf->intf_nw_props.vlans[9]);
    }
  }

  cli_print(cli, "\t MAC : %02X:%02X:%02X:%02X:%02X:%02X",
      INTF_MAC(intf)[0], INTF_MAC(intf)[1],
      INTF_MAC(intf)[2], INTF_MAC(intf)[3],
      INTF_MAC(intf)[4], INTF_MAC(intf)[5]);
}

void dump_nw_graph (graph_t *graph) {

  cli_print(cli, "Topology Name = %s", graph->topology_name);

  for (const auto& node : graph -> node_list) {
    dump_node_nw_props(node);
    for(unsigned int i = 0; i < MAX_INTF_PER_NODE; i++){
        interface_t *interface = node -> intfs[i];
        if(!interface) break;
        dump_intf_props(interface);
    }
  }
}



#endif
