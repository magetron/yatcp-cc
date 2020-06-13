#ifndef UTILS_CC
#define UTILS_CC

#include "graph.hh"

void apply_mask (ip_addr_t *prefix, unsigned char mask, ip_addr_t *str_prefix) {
	for (unsigned int i = 0; i < IP_ADDR_LENGTH; i++) {
		unsigned char ones = mask - i * 8; if (ones > 8) ones = 8;
		unsigned char m = 0;
		for (unsigned int j = ones; j > 0; j--) m += 1 << (j - 1);
		str_prefix -> addr[i] = prefix -> addr[i] & m;
	}
}

ip_addr_t *apply_mask (ip_addr_t *prefix, unsigned char mask) {
	ip_addr_t *str_prefix = new ip_addr_t();
	apply_mask(prefix, mask, str_prefix);
	return str_prefix;
}
	
void layer2_fill_with_broadcast_mac (mac_addr_t *mac_addr) {
	memset(mac_addr, 0xFF, sizeof(mac_addr_t));
}

mac_addr_t *layer2_fill_with_broadcast_mac () {
	mac_addr_t *mac_addr = new mac_addr_t();
	layer2_fill_with_broadcast_mac(mac_addr);
	return mac_addr;
}

#endif
