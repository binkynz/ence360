CC = gcc 
# CFLAGS =  -Wall -lrt -lm -O3 -funroll-loops
CFLAGS =  -Wall -lrt -lm -g --std=gnu99 -lpthread -lreadline

.PHONY: default all clean

default: client server
all: default


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 

list: list.c $(TEST_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

client: client.c $(TEST_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

server: server.c list.c list.h $(TEST_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


clean:
	-rm -f *.o 
	-rm -f list client server
