# yatcp-cc

Yet another TCP/IP stack written in crappy C-style C++

## Instructions

To Download dependencies in `libs/`, use `git submodule`:

```bash
git submodule update --init --recursive
```

To run the model, simply:

```bash
make
```

And then execute:

```bash
./test.out
```

`telnet` to the cli via

```bash
telnet localhost 10023
```

### Docker

To use `Docker` alternatively,

Build with:

```bash
docker build -t yatcp-cc .
```

And then execute:

```bash
docker run -p 10023:10023 yatcp-cc
```

## Stage 1

* Model network topologies (Layer 2 / 3)
* Integrate with [libcli](https://github.com/magetron/libcli)
* Mimic Layer 1 network among nodes in topologies

## Stage 2

* Model ARP Protocol


