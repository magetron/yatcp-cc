#ifndef NWCLI_CC
#define NWCLI_CC

#define TELNET_PORT 10023

#include "graph.hh"
#include "nwcli.hh"

#include <cstring>

extern graph_t *topo;

cli_def *cli = cli_init();

static inline int show_nw_topology_handler (cli_def *cli, const char *command, char *argv[], int argc) {
 	dump_nw_graph(topo);
	return CLI_OK;
}

static inline int run_node_arp_handler (cli_def *cli, const char *command, char *argv[], int argc) {
	if (!cli_get_optarg_value(cli, "node", nullptr) || !cli_get_optarg_value(cli, "resolve-arp", nullptr)) {
		cli_print(cli, "ERROR : missing argument");
		return CLI_ERROR;
	}
	cli_print(cli, "%s %s", cli_get_optarg_value(cli, "node", nullptr), cli_get_optarg_value(cli, "resolve-arp", nullptr));
	return CLI_OK;
}

static inline int node_validator (cli_def *cli, const char *name, const char *value) {
	for (auto& n : topo -> node_list) if (!strncmp(value, n -> node_name, NODE_NAME_SIZE)) return CLI_OK;
	cli_print(cli, "ERROR : node not found");
	return CLI_ERROR;
}

static inline int resolve_arp_validator (cli_def *cli, const char *name, const char *value) {
	unsigned int ip_d = 0;
	while (true) {
		if (*value == '.' || *value == '\0') {
			if (ip_d < 0 || ip_d > 255) {
				cli_print(cli, "ERROR : IP invalid");
				return CLI_ERROR;
			}
			ip_d = 0;
		} else if (*value >= '0' && *value <= '9') ip_d = ip_d * 10 + (*value - '0');
		else {
			cli_print(cli, "ERROR : IP invalid");
			return CLI_ERROR;
		}
		if (*value == '\0') break; else value++;
	}	
	return CLI_OK;
}

void init_nwcli () {

	cli_set_hostname(cli, "yatcp-cc");
	cli_set_banner(cli, "Welcome to Yet Another TCP CLI written by Patrick Wu.");
	
	// show *
	cli_command *show = cli_register_command(cli, nullptr, "show", nullptr, PRIVILEGE_UNPRIVILEGED, MODE_EXEC, "Show / Dump relevant data");
	// show topology
	cli_register_command(cli, show, "topology", show_nw_topology_handler, PRIVILEGE_UNPRIVILEGED, MODE_EXEC, "Show / Dump complete network topology");

	// run *
	cli_command *run = cli_register_command(cli, nullptr, "run", run_node_arp_handler, PRIVILEGE_UNPRIVILEGED, MODE_EXEC, "Run relevant node / interface");
	// run node <node-name> resolve-arp <ip-address>
	cli_optarg *node = cli_register_optarg(run, "node", CLI_CMD_OPTIONAL_ARGUMENT, PRIVILEGE_UNPRIVILEGED, MODE_EXEC, "Specify node", nullptr /* completer */, node_validator, nullptr /* transient eval */);
	cli_optarg *resolve_arp = cli_register_optarg(run, "resolve-arp", CLI_CMD_OPTIONAL_ARGUMENT, PRIVILEGE_UNPRIVILEGED, MODE_EXEC, "Specify resolve-arp", nullptr /* completer */, resolve_arp_validator, nullptr /* transient eval */);

}

void serve_nwcli () {

	sockaddr_in server_addr;
	int on = 1;
	int s = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	
	// Listen on port TELNET_PORT
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(TELNET_PORT);
	bind(s, (struct sockaddr *)&server_addr, sizeof(server_addr));

	// Wait for a connection
	listen(s, 50);
	printf("yatcp-cc listening on port %d...\n", TELNET_PORT);

	int x = 0;
	while ((x = accept(s, NULL, 0))) {
		// Pass the connection off to libcli
		cli_loop(cli, x);
		close(x);
	}
	
	cli_done(cli);
}

#endif
