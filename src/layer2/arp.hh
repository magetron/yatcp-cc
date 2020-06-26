#ifndef ARP_HH
#define ARP_HH

#include "../config.hh"

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
	mac_addr_t dst_mac;
	ip_addr_t dst_ip;
};
#pragma pack (pop)


struct arp_table_t {
	
	struct arp_table_entry_t {
		mac_addr_t mac_addr;
		char o_intf_name[INTF_NAME_SIZE]; // outgoing intf name
	};
	
	std::unordered_map<ip_addr_t, arp_table_entry_t> map;
/*	
	// Create Read Update Delete
	bool add (ip_addr_t *ip, arp_table_entry_t* arp_entry) {
		ip_addr_t aip = *ip;
		if (map.find(aip) != map.end()) return false;
		map.insert({aip, *arp_entry});
		return true;
	}

	arp_table_entry_t *lookup(ip_addr_t *ip) {
		if (map.find(*ip) == map.end()) return nullptr;
		else return &(map[*ip]);
	}

	
	bool update(arp_hdr_t *arp_hdr, char *i_intf_name) {
		assert(arp_hdr -> oper == ARP_REPLY);
		map[arp_hdr -> src_ip].mac_addr = arp_hdr -> src_mac;
		memcpy(&map[arp_hdr -> src_ip].o_intf_name, i_intf_name, INTF_NAME_SIZE);
		return true;
	}
*/



};




#endif
