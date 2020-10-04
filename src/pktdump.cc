#ifndef PKTDUMP_CC
#define PKTDUMP_CC

#include "pktdump.hh"

void dump_arp (uint8_t* pkt, uint32_t pkt_size) {

}

void dump_pkt (uint8_t* pkt, uint32_t pkt_size) {
  auto* eth_hdr = reinterpret_cast<ethernet_hdr_t*>(pkt);
  cli_print(cli, "DST MAC : %02X:%02X:%02X:%02X:%02X:%02X",
              eth_hdr->dst_addr.addr[0], eth_hdr->dst_addr.addr[1],
              eth_hdr->dst_addr.addr[2], eth_hdr->dst_addr.addr[3],
              eth_hdr->dst_addr.addr[4], eth_hdr->dst_addr.addr[5]);
  cli_print(cli, "SRC MAC : %02X:%02X:%02X:%02X:%02X:%02X",
              eth_hdr->src_addr.addr[0], eth_hdr->src_addr.addr[1],
              eth_hdr->src_addr.addr[2], eth_hdr->src_addr.addr[3],
              eth_hdr->src_addr.addr[4], eth_hdr->src_addr.addr[5]);
  cli_print(cli, "ETHERTYPE = %X", eth_hdr->ethertype);
  switch (eth_hdr->ethertype) {
    case ethernet_hdr_t::ARP_TYPE:
      dump_arp(eth_hdr->payload, pkt_size - ETH_HDR_SIZE_EXCL_PAYLOAD);
      break;
    default:
      cli_print(cli, "UNKNOWN ETHERTYPE");
  }
}


#endif