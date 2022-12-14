#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define NUM_FUNCS 3	  // number of math functions
#define NUM_THREADS 4 // number of threads that can be created (MUST be > 0)

// declare a macro to handle exceptions
#define error_exit(prompt)  \
	do                      \
	{                       \
		perror(prompt);     \
		exit(EXIT_FAILURE); \
	} while (0)

// define the signatures of the math functions
double gaussian(double);
double charge_decay(double);

// declare an array of math functions
typedef double math_func_t(double);
static math_func_t *const funcs[NUM_FUNCS] = {&sin, &gaussian, &charge_decay};

// create a struct to pass to threads
typedef struct
{
	double *area; // the total area of the integral
	pthread_mutex_t *lock;  // the mutex lock to protect the area

	double range_start, dx; // the start range, and delta x
	size_t start_step, incr_step; // the start slice, and slice increment amount
	size_t num_steps, func_id; // the number of slices, and the (integral) function id
} worker_t;

// gaussian function
double gaussian(double x)
{
	return exp(-(x * x) / 2) / (sqrt(2 * M_PI));
}

// charge decay function
double charge_decay(double x)
{
	if (x < 0)
		return 0;
	else if (x < 1)
		return 1 - exp(-5 * x);
	else
		return exp(-(x - 1));
}

// integrate using the trapezoid method.
void *integrate_trap(void *arg)
{
	worker_t *worker = (worker_t *)arg; // grab the worker argument

	math_func_t *func = funcs[worker->func_id]; // the function to integrate

	double area = 0;																   // the area of the (sub) integral
	for (size_t i = worker->start_step; i < worker->num_steps; i += worker->incr_step) // iterate over all steps in the (sub) integral
	{
		double smallx = worker->range_start + i * worker->dx;	  // the left-x value
		double bigx = worker->range_start + (i + 1) * worker->dx; // the right-x value

		area += func(smallx) + func(bigx); // the sum of the function at the x values
	}

	area = worker->dx * area / 2; // finish the area calculation

	if (pthread_mutex_lock(worker->lock) != 0) // acquire lock
		error_exit("mutex_lock");

	*worker->area += area; // increment the total area

	if (pthread_mutex_unlock(worker->lock) != 0) // release lock
		error_exit("mutex_unlock");

	return NULL; // finished
}

// get a valid input from stdin
bool get_valid_input(double *start, double *end, size_t *num_steps, size_t *func_id)
{
	// print the query
	if (printf("Query: [start] [end] [num_steps] [func_id]\n") < 0)
		error_exit("printf");

	size_t num_read = scanf("%lf %lf %zu %zu", start, end, num_steps, func_id); // read from stdin and grab the values

	return (num_read == 4 && *end >= *start && *num_steps > 0 && *func_id < NUM_FUNCS); // sanity check
}

// spawn child threads to perform the integration
void spawn_child_threads(worker_t workers[], pthread_mutex_t *lock, double *area, double range_start, double range_end, size_t num_steps, size_t func_id)
{
	*area = 0; // clear the area value

	double dx = (range_end - range_start) / num_steps; // the "step" size of each sub-integral

	pthread_t thread_ids[NUM_THREADS]; // store the thread ids

	// create a worker thread
	for (size_t i = 0; i < NUM_THREADS; i++)
	{
		worker_t *worker = &workers[i];

		worker->area = area; // (double) area reference
		worker->lock = lock; // mutex lock
		worker->range_start = range_start; // start of integral
		worker->dx = dx; // integral step size
		worker->start_step = i; // start slice
		worker->incr_step = NUM_THREADS; // number of slices to skip
		worker->num_steps = num_steps; // total number of slices
		worker->func_id = func_id; // function id

		if (pthread_create(&thread_ids[i], NULL, integrate_trap, worker) != 0) // spawn a thread in the integrate_trap with the worker initiated above
			error_exit("pthread_create");
	}

	// wait for all the threads to finish
	for (size_t i = 0; i < NUM_THREADS; i++)
		if (pthread_join(thread_ids[i], NULL) != 0)
			error_exit("pthread_join");

	// print the result
	if (printf("The integral of function %zu in range %g to %g is %.10g\n", func_id, range_start, range_end, *area) < 0)
		error_exit("printf");
}

// entry point of the program
int main(void)
{
	// we MUST define area in main...
	double area;

	// define variables populated by stdin
	double range_start, range_end;
	size_t num_steps, func_id;

	worker_t workers[NUM_THREADS]; // define an array of workers (one for each thread)

	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // create a mutex to protect writing to the total area

	// spawn the child threads
	while (get_valid_input(&range_start, &range_end, &num_steps, &func_id))
		spawn_child_threads(workers, &lock, &area, range_start, range_end, num_steps, func_id);

	exit(EXIT_SUCCESS); // finished
}