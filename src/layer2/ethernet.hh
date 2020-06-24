#ifndef ETHERNET_HH
#define ETHERNET_HH

#define ETH_HDR_SIZE_EXCL_PAYLOAD (sizeof(mac_addr_t) * 2 + sizeof(unsigned short) + sizeof(unsigned int))

struct ethernet_hdr_t {	
	//char preamble[8]; // alignment bytes with SFD, not used
	mac_addr_t dst_addr;
	mac_addr_t src_addr;
	unsigned short length; // [46..1500]
	unsigned char* payload; 
	//char payload[MAX_PAYLOAD_SIZE];
	unsigned int fcs; // checksum
	
	// TODO : impl fcs
	static unsigned int generate_fcs (unsigned char *payload, unsigned short length, mac_addr_t* d_addr, mac_addr_t* s_addr) {
		return 0;
	}

	ethernet_hdr_t () {
		payload = nullptr;
	}

	ethernet_hdr_t (unsigned short length) {
		payload = (unsigned char *)malloc(sizeof(char) * length);
	}

	ethernet_hdr_t (unsigned char *pkt, unsigned short pkt_size) {
		// ASSUMPTION : pkt_size <= MAX_PAYLOAD_SIZE
		length = pkt_size < MIN_PAYLOAD_SIZE ? MIN_PAYLOAD_SIZE : pkt_size;
		if (pkt_size < MIN_PAYLOAD_SIZE) {
			payload = (unsigned char *)malloc(sizeof(char) * MIN_PAYLOAD_SIZE);
			memcpy(payload, pkt, pkt_size);
			memset(payload + pkt_size, '\0', MIN_PAYLOAD_SIZE - pkt_size);
		} else {
			payload = (unsigned char *)malloc(sizeof(char) * pkt_size);
			memcpy(payload, pkt, pkt_size);
		}
		fcs = generate_fcs(payload, length, &dst_addr, &src_addr);
	}

	ethernet_hdr_t (mac_addr_t *d_addr, mac_addr_t *s_addr, unsigned char *pkt, unsigned short pkt_size) : ethernet_hdr_t(pkt, pkt_size) {
		memcpy(&dst_addr, d_addr, sizeof(mac_addr_t));
		memcpy(&src_addr, s_addr, sizeof(mac_addr_t));
	}

	void *get_hdr () {
		unsigned char *hdr = (unsigned char *)malloc(ETH_HDR_SIZE_EXCL_PAYLOAD + length);
		memcpy(hdr, &dst_addr, sizeof(mac_addr_t));
		memcpy(hdr + sizeof(mac_addr_t), &src_addr, sizeof(mac_addr_t));
		memcpy(hdr + 2 * sizeof(mac_addr_t), &length, sizeof(unsigned short));
		memcpy(hdr + 2 * sizeof(mac_addr_t) + sizeof(unsigned short), payload, sizeof(unsigned char) * length);
		memcpy(hdr + 2 * sizeof(mac_addr_t) + sizeof(unsigned short) + sizeof(unsigned char) * length, &fcs, sizeof(unsigned int));
		return (void *)hdr;
	}

	void restore_ethernet_hdr_t (void *plain_hdr) {
		unsigned char *hdr = (unsigned char *)plain_hdr;
		memcpy(&dst_addr, hdr, sizeof(mac_addr_t));
		memcpy(&src_addr, hdr + sizeof(mac_addr_t), sizeof(mac_addr_t));
		memcpy(&length, hdr + 2 * sizeof(mac_addr_t), sizeof(unsigned short));
		if (payload) free(payload);
		payload = (unsigned char *)malloc(sizeof(unsigned char) * length);
		memcpy(payload, hdr + 2 * sizeof(mac_addr_t) + sizeof(unsigned short), sizeof(unsigned char) * length);
		memcpy(&fcs, hdr + 2 * sizeof(mac_addr_t) + sizeof(unsigned short) + sizeof(unsigned char) * length, sizeof(unsigned int));
		free(plain_hdr);
	}	

	~ethernet_hdr_t () {
		free(payload);
	}

};


// HELPER
static inline ethernet_hdr_t *alloc_eth_hdr_with_payload (unsigned char *pkt, unsigned short pkt_size) {
	return new ethernet_hdr_t(pkt, pkt_size);
}

static inline bool l2_frame_recv_qualify_on_intf (interface_t *intf, ethernet_hdr_t *hdr) {
	// ASSUME INTF is operating in L3 mode
	if (!IS_INTF_L3_MODE(intf)) return false;

	if (memcmp(INTF_MAC(intf), hdr -> dst_addr.addr, sizeof(mac_addr_t)) == 0) return true;

	if (is_mac_broadcast_addr(&(hdr -> dst_addr))) return true;
	
	return false;
}

#endif
