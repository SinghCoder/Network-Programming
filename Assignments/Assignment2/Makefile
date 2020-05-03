# Makefile variables for the compiler and compiler flags
# To use Makefile variables later in the Makefile: $()
#
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#
CC = gcc
CFLAGS  = -g -Wall
LIBS = -pthread
# typing 'make' will invoke the first target entry in the file 
# (in this case the default target entry)
# you can name this target entry anything, but "default" or "all"
# are the most commonly used names by convention
default: server

server:  webserver.o hashtable.o helper.o
	$(CC) $(CFLAGS) $(LIBS) -o server webserver.o hashtable.o helper.o

helper.o:  helper.c helper.h
	$(CC) $(CFLAGS) -c helper.c

hashtable.o:  hashtable.c hashtable.h helper.c helper.h
	$(CC) $(CFLAGS) -c hashtable.c

webserver.o: webserver.c webserver.h helper.c helper.h hashtable.c hashtable.h
	$(CC) $(CFLAGS) -c webserver.c

# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean: 
	$(RM) server *.o *~