#ifndef TESTAPP_CC
#define TESTAPP_CC

#include "config.hh"
#include "import.hh"

#include "graph.hh"
#include "topologies.hh"
#include "comm.hh"
#include "nwcli.hh"
#include "layer2/layer2.hh"

graph_t *topo = nullptr;

static inline void network_sim_test () {
  topo = hello_world_topo();

  nw_start_pkt_receiver_thread(topo);

  //wait for pkt_receiver to start
  usleep(1e6);

  node_t *send_node = get_node_by_node_name(topo, (char *)"R0_re");
  interface_t *send_intf = get_node_intf_by_name(send_node, (char *)"eth0/0");

  uint8_t msg[] = "Hello World!";

  send_pkt(msg, 13, send_intf);
  send_pkt_flood(send_node, nullptr, msg, 13);
}

static inline void arp_impl_test () {
  topo = linear_topo();

  nw_start_pkt_receiver_thread(topo);

  //wait for pkt_receiver to start, sleep 1 second
  usleep(1e6);

  init_nwcli();
  serve_nwcli();
}

// DEBUG
static inline void check_size () {
  printf("ip_addr_t size = %lu, mac_addr_t size = %lu, ethernet_hdr_t size = %lu, ethernet_hdr_t excluding payload size = %lu, arp_hdr_t size = %lu\n", sizeof(ip_addr_t), sizeof(mac_addr_t), sizeof(ethernet_hdr_t), ETH_HDR_SIZE_EXCL_PAYLOAD, sizeof(arp_hdr_t));
}

int main (int argc, char **argv) {
  arp_impl_test();
  return 0;
}

#endif
