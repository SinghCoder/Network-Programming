## Shell

### Features implemented 

- Pipelining of commands
    ```
        ls -l | grep ".c" | wc
    ```

- Input output redirection
    ```
        ls > output.txt
    ```

- Shared Memory creation of given size
    ```
        shmcreate <size>
    ```

- Listing all shared memories
    ```
        shmls   #lists all shmems IDs with size
    ```

- Redirecting a command's output/input to/from a shared memory segment
    ```
        ls > <shmid>
    ```

- TCP Connection establishment to given ip and port
    ```
        tcpcreate <ip> <port>
        tcpcreate 127.0.0.1 12345
    ```

- Listing active TCP connections
    ```
        tcpls   # lists active tcp coxns IP, port, id of sockets
    ```

- Redirecting a command's output/input to/from server at an active tcp connection
    ```
        ls > <tcpid>
        wc < <tcpid>
    ```


### Instructions to start

- Run makefile 
    ```
        make
    ```
- Type commands at shell, for e.g.
    ```
        - ls
        - ls | wc
        - cat shell.c
        - ls > output.txt | wc | wc
    ```

### Example Image

<img src = "./example.gif">