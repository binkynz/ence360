#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define NUM_FUNCS 3
#define NUM_THREADS 4

// define the signatures of the math functions
double gaussian(double);
double charge_decay(double);

// declare an array of math functions
typedef double math_func_t(double);
static math_func_t* const funcs[NUM_FUNCS] = {&sin, &gaussian, &charge_decay};

// create a struct to pass to threads
typedef struct {
	double* area;
	pthread_mutex_t* lock;

	double range_start, range_end;
	size_t num_steps, func_id;
} worker_t;

// gaussian function
double gaussian(double x) {
	return exp(-(x * x) / 2) / (sqrt(2 * M_PI));
}

// charge decay function
double charge_decay(double x) {
	if (x < 0) 
		return 0;
	else if (x < 1)
		return 1 - exp(-5 * x);
	else
		return exp(-(x - 1));
}

// integrate using the trapezoid method. 
void* integrate_trap(void* arg) {
	worker_t* worker = (worker_t*)arg; // grab the worker argument

	double range_size = worker->range_end - worker->range_start; // the size of the integral
	double dx = range_size / worker->num_steps; // the integral increment amount

	math_func_t* func = funcs[worker->func_id]; // the function to integrate

	double area = 0; // the area of the integral
	for (size_t i = 0; i < worker->num_steps; i++) { // iterate over all steps in the integral
		double smallx = worker->range_start + i * dx; // the left-x value
		double bigx = worker->range_start + (i + 1) * dx; // the right-x value

		area += func(smallx) + func(bigx); // the sum of the function at the x values
	}

	area = dx * area / 2; // finish the area calculation

	pthread_mutex_lock(worker->lock); // acquire lock
	*worker->area += area; // increment the total area
	pthread_mutex_unlock(worker->lock); // release lock

	return NULL; // finished
}

// get a valid input from stdin
bool get_valid_input(double* start, double* end, size_t* num_steps, size_t* func_id) {
	printf("Query: [start] [end] [num_steps] [func_id]\n"); // print the query

	size_t num_read = scanf("%lf %lf %zu %zu", start, end, num_steps, func_id); // read from stdin and grab the values

	return (num_read == 4 && *end >= *start && *num_steps > 0 && *func_id < NUM_FUNCS); // sanity check
}

// spawn child threads to perform the integration
void spawn_child_threads(worker_t workers[], pthread_mutex_t* lock, double range_start, double range_end, size_t num_steps, size_t func_id) {
	double area = 0; // the total area
		
	double dx = (range_end - range_start) / NUM_THREADS; // the "step" size of each sub-integral
	size_t steps_per_thread = num_steps / NUM_THREADS;  // the number of steps for each sub-integral
	
	// if the number of steps is not a multiple of the request
    if (num_steps % NUM_THREADS != 0) steps_per_thread += 1; // add an extra step (rather have higher accuracy than less)

	pthread_t thread_ids[NUM_THREADS]; // store the thread ids

    // create a worker thread
	for (size_t i = 0; i < NUM_THREADS; i++) {
		worker_t* worker = &workers[i];

		worker->area = &area;
		worker->lock = lock;
		worker->range_start = range_start + (i * dx);
		worker->range_end = range_start + (i + 1) * dx;
		worker->num_steps = steps_per_thread;
		worker->func_id = func_id;

		pthread_create(&thread_ids[i], NULL, integrate_trap, worker); // spawn a thread in the integrate_trap with the worker initiated above
	}

    // wait for all the threads to finish
	for (size_t i = 0; i < NUM_THREADS; i++)
		pthread_join(thread_ids[i], NULL);

    // print the result
	printf("The integral of function %zu in range %g to %g is %.10g\n", 
		func_id, range_start, range_end, area);
}

// entry point of the program
int main(void) {
	// define variables populated by stdin
	double range_start, range_end;
	size_t num_steps, func_id;

	worker_t workers[NUM_THREADS]; // define an array of workers (one for each thread)
	
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // create a mutex to protect writing to the total area

	 // spawn the child threads
	while (get_valid_input(&range_start, &range_end, &num_steps, &func_id))
		spawn_child_threads(workers, &lock, range_start, range_end, num_steps, func_id);

	exit(EXIT_SUCCESS); // finished
}