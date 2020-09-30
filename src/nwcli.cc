#ifndef NWCLI_CC
#define NWCLI_CC

#include "graph.hh"
#include "nwcli.hh"
#include "layer2/layer2.hh"

extern graph_t *topo;

cli_def *cli = cli_init();

static inline int show_nw_topology_handler (cli_def *cli, const char *command, char *argv[], int argc) {
   dump_nw_graph(topo);
  return CLI_OK;
}

static inline int show_node_details_handler (cli_def *cli, const char *command, char *argv[], int argc) {
  if (!cli_get_optarg_value(cli, "node_name", nullptr) || !cli_get_optarg_value(cli, "node_detail", nullptr)) {
    cli_print(cli, "ERROR : missing argument");
    return CLI_ERROR;
  }
  char *node_name = cli_get_optarg_value(cli, "node_name", nullptr);
  char *node_detail = cli_get_optarg_value(cli, "node_detail", nullptr);

  node_t *node = get_node_by_node_name(topo, node_name);
  if (!strncmp(node_detail, "arp", 4)) dump_arp_table(node->node_nw_props.arp_table);
  else if (!strncmp(node_detail, "name", 5)) cli_print(cli, "node name = %s", node->node_name);
  else if (!strncmp(node_detail, "mac", 4)) dump_mac_table(node->node_nw_props.mac_table);
  else cli_print(cli, "<node-detail> shall either be arp, name or mac");
  return CLI_OK;
}

static inline int run_node_resolve_arp_handler (cli_def *cli, const char *command, char *argv[], int argc) {
  if (!cli_get_optarg_value(cli, "node_name", nullptr) || !cli_get_optarg_value(cli, "ip_address", nullptr)) {
    cli_print(cli, "ERROR : missing argument");
    return CLI_ERROR;
  }
  char *node_name = cli_get_optarg_value(cli, "node_name", nullptr);
  char *ip_literal = cli_get_optarg_value(cli, "ip_address", nullptr);

  node_t *node = get_node_by_node_name(topo, node_name);
  ip_addr_t *ip = string_to_ip_addr_t(ip_literal);
  send_arp_broadcast_request(node, nullptr, string_to_ip_addr_t(ip_literal));
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
  // show node
  cli_command *show_node = cli_register_command(cli, show, "node", show_node_details_handler, PRIVILEGE_UNPRIVILEGED, MODE_EXEC, "Show / Dump node info");
  // show node <node-name> <node-detail>
  cli_optarg *show_node_name = cli_register_optarg(show_node, "node_name", CLI_CMD_ARGUMENT, PRIVILEGE_UNPRIVILEGED, MODE_EXEC, "Specify node", nullptr /* completer */, node_validator, nullptr /* transient eval */);
  cli_optarg *show_node_detail = cli_register_optarg(show_node, "node_detail", CLI_CMD_ARGUMENT, PRIVILEGE_UNPRIVILEGED, MODE_EXEC, "Specify node detail", nullptr, nullptr, nullptr);

  // run *
  cli_command *run = cli_register_command(cli, nullptr, "run", nullptr, PRIVILEGE_UNPRIVILEGED, MODE_EXEC, "Run relevant node / interface");
  // run node
  cli_command *run_node = cli_register_command(cli, run, "node", nullptr, PRIVILEGE_UNPRIVILEGED, MODE_EXEC, "Run node instructions");
  // run node resolve-arp <node-name> <ip-address>
  cli_command *run_node_resolve_arp = cli_register_command(cli, run_node, "resolve-arp", run_node_resolve_arp_handler, PRIVILEGE_UNPRIVILEGED, MODE_EXEC, "Run node resolve-arp");
  cli_optarg *run_node_resolve_arp_name = cli_register_optarg(run_node_resolve_arp, "node_name", CLI_CMD_ARGUMENT, PRIVILEGE_UNPRIVILEGED, MODE_EXEC, "Specify node", nullptr /* completer */, node_validator, nullptr /* transient eval */);
  cli_optarg *run_node_resolve_arp_ip = cli_register_optarg(run_node_resolve_arp, "ip_address", CLI_CMD_ARGUMENT, PRIVILEGE_UNPRIVILEGED, MODE_EXEC, "Specify IP address", nullptr /* completer */, nullptr /* validator */, nullptr /* transient eval */);
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
