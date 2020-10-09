# yatcp-cc

Yet another TCP/IP stack written in crappy C-style C++

## Screenshot

![Topology](https://raw.githubusercontent.com/magetron/yatcp-cc/master/imgs/1.png)

![Topology-Switches](https://raw.githubusercontent.com/magetron/yatcp-cc/master/imgs/2.png)

![ARP-Resolution](https://raw.githubusercontent.com/magetron/yatcp-cc/master/imgs/3.png)

## Instructions

To Download dependencies in `libs/`, use `git submodule`:

```bash
git submodule update --init --recursive
```

To run the model, simply:

```bash
make all
```

And then execute:

```bash
./test.out
```

`telnet` to the cli via

```bash
telnet localhost 10023
```

## Stage 1

* Model network topologies (Layer 2 / 3)
* Integrate with [libcli](https://github.com/magetron/libcli)
* Mimic Layer 1 network among nodes in topologies

## Stage 2

* Model ARP Protocol
* Layer 2 Switching
* VLAN aware L2 Switches

## TODO

* TCP/IP
* L3 routing
* On Demand ARP Resolution
