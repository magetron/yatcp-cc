#ifndef PKTDUMP_CC
#define PKTDUMP_CC

#include "pktdump.hh"

// TODO : impl
void dump_pkt (uint8_t* pkt, uint32_t pkt_size) {
  auto* eth_hdr = reinterpret_cast<ethernet_hdr_t*>(pkt);
  cli_print(cli, "");
}


#endif