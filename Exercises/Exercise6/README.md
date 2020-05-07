# Concurrent TCP Server using signal-driven I/O

### Task Requirement:
Write a program [webserver.c](./webserver.c) using which works in the following way:
- It

- Establish signal handler for SIGIO using `signal()` and `sigaction()`
- Set socket owner. `fcntl()` with `F_SETOWN`. It tells who will receive signals
    - `fcntl(fd, F_SETOWN, getpid());`
- Enable signal driven I/O for the socket using `O_ASYNC`
- Make connection sockets to be non-blocking bcz u get edge-triggered notifications. So you have to read all available data by  performs I/O system calls until a call fails with the error `EAGAIN` or `EWOULDBLOCK`.
    - use `O_NONBLOCK` flag.
    - `fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_ASYNC | O_NONBLOCK);`
- Specify a realtime signal to be used in place of SIGIO to avoid queuing and handling multiple connections.
- Use `SA_SIGINFO` flag in `sigaction()` to establish handler for RT signals.
### Server performance

### Demo Run

### Dependencies:

### Instructions to run:

