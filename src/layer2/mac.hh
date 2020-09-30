#ifndef MAC_HH
#define MAC_HH

#include "../config.hh"

#include "l2config.hh"
#include "l2import.hh"

#include "../nwcli.hh"

extern cli_def* cli;

struct mac_table_t {

  struct mac_table_entry_t {
    mac_addr_t mac_addr;
    char o_intf_name[INTF_NAME_SIZE];
    mac_table_entry_t () { }

    mac_table_entry_t (mac_addr_t* mac, char* intf_name) {
      memcpy(&mac_addr, mac, sizeof(mac_addr_t));
      memcpy(&o_intf_name, intf_name, INTF_NAME_SIZE);
    }

    bool operator == (const mac_table_entry_t& entry) {
      return mac_addr == entry.mac_addr && !(memcmp(o_intf_name, entry.o_intf_name, INTF_NAME_SIZE));
    }
  };

  std::list<mac_table_entry_t> entries;

  bool add (mac_addr_t* mac, char* intf_name) {
    if (lookup(mac) != nullptr) return false;
    entries.push_back({mac, intf_name});
    return true;
  }

  mac_table_entry_t* lookup(mac_addr_t* mac) {
    for (auto& entry : entries) {
      if (entry.mac_addr == *mac) return &entry;
    }
    return nullptr;
  }

  bool update (mac_addr_t* mac, char* intf_name) {
    mac_table_entry_t* entry = lookup(mac);
    if (entry == nullptr) return false;
    memcpy(&entry->o_intf_name, intf_name, INTF_NAME_SIZE);
  }

  void remove (mac_addr_t* mac) {
    mac_table_entry_t* entry = lookup(mac);
    entries.remove(*entry);
  }
};
#endif