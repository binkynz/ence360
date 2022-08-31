#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER; // initialised to unlocked state

// global int to store the data
int global_data = 0;

// predefine function signatures
void read_data();
void set_data();

int main()
{
	// create variables to store the thead ids, thread return values
	pthread_t thread1, thread2;
    int thread_return1, thread_return2;
	// instantiate a counter for the for loop
	int i;
    
	// loop 5 times
    for(i = 0; i < 5; i++) {
		// create two threads, one running in set_data and the other in read_data
		thread_return1 = pthread_create( &thread1, NULL, (void*)&set_data, NULL);
     	thread_return2 = pthread_create( &thread2, NULL, (void*)&read_data, NULL);
	}
    
	// wait for threads to finish before continuing
	pthread_join(thread1, NULL); // wait for thread to return from thread1 id
	pthread_join(thread2, NULL); // wait for thread to return from thread2 id

	// print that we are exiting
    printf("exiting\n");
	// exit
    exit(0);
}

void set_data()
{
	// protect critical region:
	
	// acquire mutex
	pthread_mutex_lock(&mutex1);

	// print that we are setting the data
	printf("Setting data\t");
	// update the global variable to a random number
	global_data = rand();

	// release mutex
	pthread_mutex_unlock(&mutex1);
}

void read_data()
{
	// variable to store the global variable value
	int data;

	// protect critical region:
	// acquire mutex
	pthread_mutex_lock(&mutex1);

	// copy the global variable value
	data = global_data;
	// print the value of the global variable
	printf("Data: %d\n", data);

	// release mutex
	pthread_mutex_unlock(&mutex1);
}


/* Run the program and write down the results displayed:

Setting data    Data: 1804289383
Setting data    Data: 846930886
Setting data    Data: 1681692777
Setting data    Data: 1714636915
Setting data    Data: 1957747793
exiting

*/
