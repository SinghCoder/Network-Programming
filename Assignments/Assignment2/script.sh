gcc -pthread -o server hashtable.c helper.c webserver.c 
gcc -o client client.c
./server