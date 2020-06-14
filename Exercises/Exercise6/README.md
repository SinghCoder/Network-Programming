# Concurrent TCP Server using signal-driven I/O

## Assumptions

- Server sends hello to all the clients and expects a hello in response
  - because then otherwise then the responses of msg sent to check if client is alive will be sent to all other clients as well