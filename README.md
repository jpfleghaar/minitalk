# 📡 minitalk

A client and a server that exchange text using only the UNIX signals `SIGUSR1` and `SIGUSR2`.

*This project was created as part of the 42 curriculum by jpflegha.*

## Description

The **server** prints its PID and waits. The **client** takes that PID and a message and sends the message to the server **one bit at a time**: `SIGUSR1` means 1, `SIGUSR2` means 0. The server rebuilds the bits into characters and prints the message.

How a message is sent (see `client.c` and `server.c`):

1. **Length first:** the client sends the message length as 32 bits. When all 32 bits have arrived, the server allocates a buffer of exactly that size.
2. **Characters:** each character is sent as 8 bits, most significant bit first, followed by a final `'\0'` byte.
3. **Acknowledgement:** after every bit the server answers with `SIGUSR1`. The client waits for this answer before sending the next bit, so no signals are lost even for long messages.
4. **End:** when the server gets the `'\0'` byte, it prints the message and sends `SIGUSR2`. The client then prints `Message received` and exits.

The server uses `sigaction` with `SA_SIGINFO` to read the sender's PID (`si_pid`), so it knows where to send the acknowledgements. It stays running and can receive any number of messages, one client after another.

## Key concepts

- UNIX signals, `sigaction`, `SA_SIGINFO`, signal masks
- Bit manipulation (encoding and decoding bytes and a 32-bit length)
- A simple handshake protocol to make signal delivery reliable
- `volatile sig_atomic_t` for state shared with a signal handler
- Dynamic memory in a signal-driven program

## Build and run

```bash
git clone https://github.com/jpfleghaar/minitalk.git
cd minitalk
make
```

This builds two programs, `server` and `client`.

Start the server in one terminal:

```bash
$ ./server
PID: 28926
Hello from minitalk
Grüße 👋 unicode
```

Send messages from a second terminal:

```bash
$ ./client 28926 "Hello from minitalk"
Message received
$ ./client 28926 "Grüße 👋 unicode"
Message received
```

Unicode works because the message is sent byte by byte. If the arguments are wrong, the client prints `Usage: [server_pid] [message]`.

Other Makefile targets: `make clean`, `make fclean`, `make re`.

## Project structure

```
minitalk/
├── Makefile
├── minitalk.h   # includes and prototypes
├── server.c     # receives bits, rebuilds length and characters, prints the message
├── client.c     # sends length and message bit by bit, waits for acknowledgements
└── utils.c      # sigaction setup, kill wrapper with error check, strlen, PID printing
```

## What I learned

- Signals are not queued: if a second signal of the same type arrives before the first is handled, it is lost. An acknowledgement after each bit solves this.
- Sending the length first lets the server allocate the buffer once instead of growing it for every character.
- Static variables inside signal handlers can keep the receiving state between calls, but they must be reset correctly between messages.
- Only a few operations are safe inside a signal handler, so the client's handler just sets a `sig_atomic_t` flag.
