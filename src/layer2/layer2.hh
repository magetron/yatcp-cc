#ifndef LAYER2_HH
#define LAYER2_HH

#include "l2config.hh"

#include "../net.hh"

struct ethernet_hdr_t {	
	//char preamble[8]; // alignment bytes with SFD, not used
	mac_addr_t dst_addr;
	mac_addr_t src_addr;
	unsigned short length; // [46..1500]
	// TODO : Think about how to impl this dynamic payload
	unsigned char* payload; 
	//char payload[MAX_PAYLOAD_SIZE];
	unsigned int fcs; // checksum
	
	static unsigned int generate_fcs (unsigned char *payload, unsigned short length, mac_addr_t* d_addr, mac_addr_t* s_addr) {
		return 0;
	}

	ethernet_hdr_t () { }

	ethernet_hdr_t (unsigned short length) {
		payload = malloc(sizeof(char) * length);
	}

	ethernet_hdr_t (mac_addr_t *d_addr, mac_addr_t *s_addr, unsigned char *pkt, unsigned short pkt_size) {
		memcpy(&dst_addr, d_addr, sizeof(mac_addr_t));
		memcpy(&src_addr, s_addr, sizeof(mac_addr_t));
		length = pkt_size;
		payload = pkt;
		fcs = generate_fcs(payload, length, &dst_addr, &src_addr);
	}

	void *get_hdr () {
		void *hdr = malloc(ETH_HDR_SIZE_EXCL_PAYLOAD + LENGTH);
		memcpy(&hdr, &dst_addr, sizeof(mac_addr_t));
		memcpy(&hdr + sizeof(mac_addr_t), &src_addr, sizeof(mac_addr_t));
		memcpy(&hdr + 2 * sizeof(mac_addr_t), &length, sizeof(unsigned short));
		memcpy(&hdr + 2 * sizeof(mac_addr_t) + sizeof(unsigned short), payload, sizeof(unsigned char) * length);
		memcpy(&hdr + 2 * sizeof(mac_addr_t) + sizeof(unsigned short) + sizeof(unsigned char) * length, fcs, sizoef(unsigned int));
		return hdr;
	}

};


// HELPER
#define ETH_HDR_SIZE_EXCL_PAYLOAD (sizeof(ethernet_hdr_t) - sizeof(((ethernet_hdr_t *)0)->payload))

static inline ethernet_hdr_t *alloc_eth_hdr_with_payload (unsigned char *pkt, unsigned short pkt_size) {
	return nullptr;
}


#endif
