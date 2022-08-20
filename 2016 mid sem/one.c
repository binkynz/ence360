#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *print_message_function(void *ptr);

int main()
{
	// store the thread id
	pthread_t id;
	// create a child thread
	pthread_create(&id, NULL, print_message_function, NULL);

	sleep(1);
	return(0);
}

void *print_message_function(void *ptr)
{
	// introduce a significant delay in the child so the parent thread finishes first
	sleep(5);
	printf("child thread\n");
	pthread_exit(NULL); 
}

/*
If the parent thread finishes before the child thread, the child thread does not continue.
*/