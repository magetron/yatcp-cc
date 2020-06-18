# yatcp-cc

Yet Another TCP/IP stack written in crappy C++.

## Instructions

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
