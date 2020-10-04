#ifndef PKTDUMP_CC
#define PKTDUMP_CC

#include "pktdump.hh"

void dump_arp (uint8_t* pkt, uint32_t pkt_size) {
  auto* arp_hdr = reinterpret_cast<arp_hdr_t*>(pkt);
  cli_print(cli, "\t ARP HDR");
  cli_print(cli, "\t h_type = %hu, p_type = %hu",
                  arp_hdr->h_type, arp_hdr->p_type);
  cli_print(cli, "\t h_len = %hu, p_type = %hu",
                  (uint16_t)arp_hdr->h_len, (uint16_t)arp_hdr->p_len);
  cli_print(cli, "\t oper = %hu", arp_hdr->oper);
  cli_print(cli, "\t src_ip = %u.%u.%u.%u",
            arp_hdr->src_ip.addr[0], arp_hdr->src_ip.addr[1],
            arp_hdr->src_ip.addr[2], arp_hdr->src_ip.addr[3]);
  cli_print(cli, "\t dst_ip = %u.%u.%u.%u",
            arp_hdr->dst_ip.addr[0], arp_hdr->dst_ip.addr[1],
            arp_hdr->dst_ip.addr[2], arp_hdr->dst_ip.addr[3]);
}

void dump_pkt (uint8_t* pkt, uint32_t pkt_size) {
  auto* eth_hdr = reinterpret_cast<ethernet_hdr_t*>(pkt);
  cli_print(cli, "ETH HDR");
  cli_print(cli, "DST MAC : %02X:%02X:%02X:%02X:%02X:%02X",
              eth_hdr->dst_addr.addr[0], eth_hdr->dst_addr.addr[1],
              eth_hdr->dst_addr.addr[2], eth_hdr->dst_addr.addr[3],
              eth_hdr->dst_addr.addr[4], eth_hdr->dst_addr.addr[5]);
  cli_print(cli, "SRC MAC : %02X:%02X:%02X:%02X:%02X:%02X",
              eth_hdr->src_addr.addr[0], eth_hdr->src_addr.addr[1],
              eth_hdr->src_addr.addr[2], eth_hdr->src_addr.addr[3],
              eth_hdr->src_addr.addr[4], eth_hdr->src_addr.addr[5]);
  cli_print(cli, "ETHERTYPE = 0x%X", eth_hdr->ethertype);
  switch (eth_hdr->ethertype) {
    case ethernet_hdr_t::ARP_TYPE:
      dump_arp(eth_hdr->payload, pkt_size - ETH_HDR_SIZE_EXCL_PAYLOAD);
      break;
    default:
      cli_print(cli, "UNKNOWN ETHERTYPE");
  }
}


#endif