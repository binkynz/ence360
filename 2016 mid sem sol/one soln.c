#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *print_message_function( void *ptr );

int main()
{
	pthread_t thread1;

	if (pthread_create( &thread1, NULL, print_message_function, NULL) != 0)
		perror("thread create error");

	sleep(1);
	return(0);
}

void *print_message_function( void *ptr )
{
	printf("child thread\n");

	sleep(5);
	printf("Hello World\n");

	pthread_exit(NULL); 
}
