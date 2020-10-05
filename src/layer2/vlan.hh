#ifndef VLAN_HH
#define VLAN_HH

#define VLAN_ETH_HDR_SIZE_EXCL_PAYLOAD ETH_HDR_SIZE_EXCL_PAYLOAD + sizeof(vlan_8021q_hdr_t)

extern cli_def *cli;

#pragma pack (push, 1)
struct vlan_8021q_hdr_t {
  uint16_t tpid;      // 0x8100
  unsigned pcp : 3;   // not used
  unsigned dei : 1;   // not used
  unsigned vid : 12;  // vlan ID

  uint32_t getVlanID () {
    return (uint32_t)vid;
  }
};
#pragma pack(pop)

#pragma pack (push, 1)
struct vlan_eth_hdr_t {
  mac_addr_t dst_addr;
  mac_addr_t src_addr;
  vlan_8021q_hdr_t vlan_hdr;
  uint16_t ethertype;
  uint8_t payload[MAX_PAYLOAD_SIZE];
  uint32_t fcs;
};
#pragma pack(pop)

static inline vlan_8021q_hdr_t* get_vlan_hdr (ethernet_hdr_t* eth_hdr) {
  if (eth_hdr->ethertype == 0x8100) {
    return reinterpret_cast<vlan_8021q_hdr_t*>(&eth_hdr->ethertype);
  } else {
    return nullptr;
  }
}

static inline uint8_t* get_eth_payload (ethernet_hdr_t* eth_hdr) {
  if (get_vlan_hdr(eth_hdr)) {
    return reinterpret_cast<vlan_eth_hdr_t*>(eth_hdr)->payload;
  } else {
    return eth_hdr->payload;
  }
}

static inline size_t get_eth_hdr_size_excl_payload (ethernet_hdr_t* eth_hdr) {
  if (get_vlan_hdr(eth_hdr)) {
    return VLAN_ETH_HDR_SIZE_EXCL_PAYLOAD;
  } else {
    return ETH_HDR_SIZE_EXCL_PAYLOAD;
  }
}

#endif