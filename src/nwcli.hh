#ifndef NWCLI_HH
#define NWCLI_HH

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

extern "C" {
	#include "../libs/libcli/libcli.h"
}

void init_nwcli();

void serve_nwcli();

#endif
