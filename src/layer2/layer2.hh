#ifndef LAYER2_HH
#define LAYER2_HH

#include "l2config.hh"

#include "../net.hh"

#pragma pack (push, 1)
struct ethernet_hdr_t {	
	//char preamble[8]; // alignment bytes with SFD, not used
	mac_addr_t dst_addr;
	mac_addr_t src_addr;
	unsigned short length; // [46..1500]
	// TODO : Think about how to impl this dynamic payload
	unsigned char payload[MIN_PAYLOAD_SIZE];
	//char payload[MAX_PAYLOAD_SIZE];
	unsigned int fcs; // checksum
};
#pragma pack (pop)

// HELPER
#define ETH_HDR_SIZE_EXCL_PAYLOAD (sizeof(ethernet_hdr_t) - sizeof(((ethernet_hdr_t *)0)->payload))

static inline ethernet_hdr_t *alloc_eth_hdr_with_payload (char *pkt, unsigned short pkt_size) {
	return nullptr;
}


#endif
