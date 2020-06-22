#ifndef L2CONFIG_HH
#define L2CONFIG_HH

// ethernet header frame
#define MAX_PAYLOAD_SIZE 1500
#define MIN_PAYLOAD_SIZE 46

#define ETH_HDR_SIZE_EXCL_PAYLOAD (sizeof(ethernet_hdr_t) - sizeof(((ethernet_hdr_t *)0)->payload))



#endif
