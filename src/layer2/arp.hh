#ifndef ARP_HH
#define ARP_HH

#include "../config.hh"

#include "l2config.hh"
#include "l2import.hh"

#include "../nwcli.hh"

extern cli_def *cli;

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

  static const uint8_t ARP_BROADCAST_REQ = 1;
  static const uint8_t ARP_REPLY = 2;
};
#pragma pack (pop)


struct arp_table_t {

  struct arp_table_entry_t {
    mac_addr_t mac_addr;
    char o_intf_name[INTF_NAME_SIZE]; // outgoing intf name
    arp_table_entry_t () { }

    arp_table_entry_t (mac_addr_t *mac, char *intf_name) {
      memcpy(&mac_addr, mac, sizeof(mac_addr_t));
      memcpy(&o_intf_name, intf_name, INTF_NAME_SIZE);
    }
  };

  std::unordered_map<ip_addr_t, arp_table_entry_t> map;

  // Create Read Update Delete
  bool add (ip_addr_t *ip, mac_addr_t *mac, char *intf_name) {
    if (map.find(*ip) != map.end()) return false;
    map.insert({*ip, arp_table_entry_t(mac, intf_name)});
    return true;
  }

  arp_table_entry_t *lookup(ip_addr_t *ip) {
    if (map.find(*ip) == map.end()) return nullptr;
    else return &map[*ip];
  }

  bool update(arp_hdr_t *arp_hdr, char *i_intf_name) {
    if (map.find(arp_hdr -> src_ip) == map.end()) return false;
    assert(arp_hdr -> oper == arp_hdr_t::ARP_REPLY);
    arp_table_entry_t *entry = new arp_table_entry_t();
    memcpy(&entry -> mac_addr, &arp_hdr -> src_mac, sizeof(mac_addr_t));
    memcpy(&entry -> o_intf_name, i_intf_name, sizeof(INTF_NAME_SIZE));
    map[arp_hdr -> src_ip] = *entry;
    return true;
  }

  void remove(ip_addr_t *ip) {
    map.erase(*ip);
  }

};

#endif
