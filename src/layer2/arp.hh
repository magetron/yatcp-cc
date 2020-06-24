#ifndef ARP_HH
#define ARP_HH

#include "l2config.hh"
#include "l2import.hh"

#pragma pack (push, 1)
struct arp_hdr_t {
	unsigned short h_type;
	unsigned short p_type;
	unsigned char h_len;
	unsigned char p_len;
	unsigned short oper;
	mac_addr_t src_mac;
	ip_addr_t src_ip;
	mac_addr_t dest_mac;
	ip_addr_t dst_ip;
};
#pragma pack (pop)


#endif
