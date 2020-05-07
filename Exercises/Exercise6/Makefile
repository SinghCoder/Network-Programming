all: server client
server: server.c server.h helper.c helper.h hashtable.c hashtable.h
	gcc -o server server.c helper.c hashtable.c

client: client.c helper.c helper.h
	gcc -o client client.c helper.c

clean: 
	rm server client