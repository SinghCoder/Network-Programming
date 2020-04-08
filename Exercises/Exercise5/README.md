## Polling server

- The server can handle upto N connections, N is given as command line argument
- It checks if listening socket is readable, accepts the connection.
- If the number of connections exceeds N, it closes this connection, else adds it to list
- It reads the data sent by any readable socket (active client sending data or close request/FIN msg)
    - if it's a close request, removes that client from the list.
    - else, forwards the data to all the other clients.

## Demo

<img src="demo.gif">