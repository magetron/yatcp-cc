#ifndef L2CONFIG_HH
#define L2CONFIG_HH

// ethernet header frame
#define MAX_PAYLOAD_SIZE 1500
#define MIN_PAYLOAD_SIZE 46

// const
#define ETHERTYPE_IPV4 0x0800
#define ETHERTYPE_ARP 0x0806
#define ETHERTYPE_IPV6 0x86DD

#define ARP_BROADCAST_REQ 1
#define ARP_REPLY 2

#endif
