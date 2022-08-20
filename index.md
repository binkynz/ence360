
## Generic
```C
```Compile```
gcc *.c -o * --std=c99 -lpthread

```Compile object file```
gcc *.c -c -o *.o

```Link object file```
gcc *.c -o * obj.o
```

## Processes and Threads
A thread is a sequence of instructions executed by a program. Every process contains at least one thread. A multi-threaded process is associated with two or more threads. All threads share the common address space of the process, and most process resources (code, data, open files, etc.) but are executed independently. Each thread has its own program counter and a stack to keep track of the local variables and return addresses.

A process can spawn a child process using ```fork```. The child process is a complete copy of the parent process. When a child process exists, it is not immediately cleared off the process table. Instead, a signal is sent to its parent process, which **must** acknowledge the child's death. Then, and only then, is the child process completely removed from the system. The period in which the child has exited and the parent has not acknowledged the child's death, the child process is in a *zombie* state.

The ```wait``` system call acknowledges a child's death. The ```wait``` call is blocking but can be performed asynchronously by listening for a ```SIGCLD``` signal (next section).
### Useful Links
```
```Create a thread```
https://man7.org/linux/man-pages/man3/pthread_create.3.html

```Create and use mutex```
https://www.thegeekstuff.com/2012/05/c-mutex-examples/

```Create a child process (fork)```
https://man7.org/linux/man-pages/man2/fork.2.html
https://man7.org/linux/man-pages/man2/wait.2.html

```Semaphore```
https://www.geeksforgeeks.org/use-posix-semaphores-c/
https://man7.org/linux/man-pages/man3/sem_init.3.html
https://man7.org/linux/man-pages/man3/sem_wait.3.html
```

## Signals, Pipes and File Descriptors
Signals are a primitive way of communicating information between processes. They occur asynchronously. 

Pipes are uni-directional and are created using ```pipe```. ```pipe``` takes an array of length 2 (int) and returns the file-desriptors of the input (indexed by 0) and output (indexed by 1) of the pipe. For bi-directional communication, two pipes must be created.

A process maintains a table of open file descriptors. When an ```open``` system call is executed, it returns a non-negative integer (file descriptor) that is an index to an entry in the process's table of open file descriptors. The ```close``` function, closes a file descriptor, so that it no longer refers to any file and may be reused. If, after a close, no file descriptors refer to an underlying open file description, the associated resources are freed.
### Useful Links
```
```Signal```
https://man7.org/linux/man-pages/man2/signal.2.html

```Pipe```
https://linux.die.net/man/2/pipe

```Duplicate file descriptor```
https://man7.org/linux/man-pages/man2/dup.2.html

```Explanation of file descriptors```
https://man7.org/linux/man-pages/man2/open.2.html
```

## Sockets
Sockets are like pipes but are bi-directional and can be used for networking. 
### Useful Links
```
```Get address info (example of client)```
https://man7.org/linux/man-pages/man3/getaddrinfo.3.html

```Bind (example of server)```
https://man7.org/linux/man-pages/man2/bind.2.html
https://www.gta.ufrj.br/ensino/eel878/sockets/sockaddr_inman.html

```TCP Sockets```
https://www.binarytides.com/socket-programming-c-linux-tutorial/
```

## Makefile
```Makefile
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
```
