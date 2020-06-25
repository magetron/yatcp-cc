#ifndef ETHERNET_HH
#define ETHERNET_HH

#define ETH_HDR_SIZE_EXCL_PAYLOAD (sizeof(mac_addr_t) * 2 + sizeof(unsigned short) + sizeof(unsigned int))

#pragma pack (push, 1)
struct ethernet_hdr_t {	
	//char preamble[8]; // alignment bytes with SFD, not used
	mac_addr_t dst_addr;
	mac_addr_t src_addr;
	unsigned short ethertype; // [1536..65535]
	unsigned char payload[MAX_PAYLOAD_SIZE]; // [46..1500] 
	unsigned int fcs; // checksum
	
	// TODO : impl fcs
	static unsigned int generate_fcs (unsigned char *payload, unsigned short _payload_length, mac_addr_t* d_addr, mac_addr_t* s_addr) {
		return 0;
	}
};
#pragma pack(pop)


// HELPER
static inline ethernet_hdr_t *alloc_eth_hdr_with_payload (unsigned char *pkt, unsigned short pkt_size) {
	
}

static inline bool l2_frame_recv_qualify_on_intf (interface_t *intf, ethernet_hdr_t *hdr) {
	// ASSUME INTF is operating in L3 mode
	if (!IS_INTF_L3_MODE(intf)) return false;

	if (memcmp(INTF_MAC(intf), hdr -> dst_addr.addr, sizeof(mac_addr_t)) == 0) return true;

	if (is_mac_broadcast_addr(&(hdr -> dst_addr))) return true;
	
	return false;
}

#endif
