#ifndef PKTDUMP_HH
#define PKTDUMP_HH

#include "config.hh"
#include "import.hh"

#include "layer2/layer2.hh"

void dump_arp (uint8_t* pkt, uint32_t pkt_size);

void dump_pkt (uint8_t* pkt, uint32_t pkt_size);

#endif