#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *print_message_function( void *ptr );

int main()
{
	// store the thread id
	pthread_t id;
	// create a thread in the prin_message_function function
	pthread_create(&id, NULL, print_message_function, NULL);

	sleep(1);
	return(0);
}

void *print_message_function( void *ptr )
{
	printf("child thread\n");
	
	// 5 second delay
	sleep(5);
	// print any output
	printf("5 seconds have passed\n");

	pthread_exit(NULL); 
}
