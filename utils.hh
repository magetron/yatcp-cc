#ifndef UTILS_HH
#define UTILS_HH

void apply_mask (ip_addr_t *prefix, unsigned char mask, ip_addr_t *str_prefix);

ip_addr_t *apply_mask (ip_addr_t *prefix, unsigned char mask);

void layer2_fill_with_broadcast_mac (mac_addr_t *mac_addr);

mac_addr_t *layer2_fill_with_broadcast_mac ();

bool is_mac_broadcast_addr (mac_addr_t *mac_addr);

#endif 
