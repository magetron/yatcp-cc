#ifndef UTILS_HH
#define UTILS_HH

#include "graph.hh"

static inline void apply_mask (ip_addr_t *prefix, unsigned char mask, ip_addr_t *str_prefix) {
	for (unsigned int i = 0; i < IP_ADDR_LENGTH; i++) {
		unsigned char ones = mask - i * 8; if (ones > 8) ones = 8;
		unsigned char m = 0;
		for (unsigned int j = ones; j > 0; j--) m += 1 << (j - 1);
		str_prefix -> addr[i] = prefix -> addr[i] & m;
	}
}

static inline ip_addr_t *apply_mask (ip_addr_t *prefix, unsigned char mask) {
	ip_addr_t *str_prefix = new ip_addr_t();
	apply_mask(prefix, mask, str_prefix);
	return str_prefix;
}

static inline void layer2_fill_with_broadcast_mac (mac_addr_t *mac_addr) {
	memset(mac_addr, 0xFF, sizeof(mac_addr_t));
}

static inline mac_addr_t *layer2_fill_with_broadcast_mac () {
	mac_addr_t *mac_addr = new mac_addr_t();
	layer2_fill_with_broadcast_mac(mac_addr);
	return mac_addr;
}

static inline bool is_mac_broadcast_addr (mac_addr_t *mac_addr) {
	for (unsigned int i = 0; i < MAC_ADDR_LENGTH; i++)
		if (mac_addr -> addr[i] != 0xFF) return false;
	return true;
}

#endif
