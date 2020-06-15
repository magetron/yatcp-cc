#ifndef NWCLI_CC
#define NWCLI_CC

#include "graph.hh"
#include "nwcli.hh"

extern graph_t *topo;

cli_def *cli = cli_init();

static inline int show_nw_topology_handler (cli_def *cli, const char *command, char *argv[], int argc) {
 	dump_nw_graph(topo);
	return CLI_OK;
}

void init_nwcli () {

	cli_set_hostname(cli, "yatcp-cc");
	cli_set_banner(cli, "Welcome to Yet Another TCP written by Patrick Wu.");
	
	// show *
	cli_command *show = cli_register_command(cli, nullptr, "show", nullptr, PRIVILEGE_UNPRIVILEGED, MODE_EXEC, "Show / Dump relevant data");
	// show topology
	cli_register_command(cli, show, "topology", show_nw_topology_handler, PRIVILEGE_UNPRIVILEGED, MODE_EXEC, "Show / Dump complete network topology");
}

void serve_nwcli () {

	sockaddr_in server_addr;
	int on = 1;
	int s = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	
	// Listen on port 12345
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(12345);
	bind(s, (struct sockaddr *)&server_addr, sizeof(server_addr));

	// Wait for a connection
	listen(s, 50);

	int x = 0;
	while ((x = accept(s, NULL, 0))) {
		// Pass the connection off to libcli
		cli_loop(cli, x);
		close(x);
	}
	
	cli_done(cli);
}

#endif
