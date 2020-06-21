#ifndef LAYER2_HH
#define LAYER2_HH

#include "l2config.hh"

#include "../net.hh"

#pragma pack (push, 1)
struct ethernet_hdr_t {	
	//char preamble[8]; // alignment bytes with SFD, not used
	mac_addr_t dst_addr;
	mac_addr_t src_addr;
	unsigned short length; // [0..1500]
	char payload[MIN_PAYLOAD_SIZE];
	//char payload[MAX_PAYLOAD_SIZE];
	unsigned int crc; // checksum
};
#pragma pack (pop)

#endif
