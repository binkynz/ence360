#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <pthread.h>

#define NUM_FUNCS 3
#define NUM_CHILDREN 4
#define CHILDREN_NAME "/children_sem"
#define NUM_THREADS 4

// define the signatures of the math functions
double gaussian(double);
double charge_decay(double);

// declare an array of math functions
typedef double math_func_t(double);
static math_func_t* const funcs[NUM_FUNCS] = {&sin, &gaussian, &charge_decay};

// declare counter of child processes
static volatile sig_atomic_t num_children = 0;

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

// create a named semaphore
sem_t* init_named_sem(void) {
    /*
        create a named semaphore with name "CHILDREN_NAME"
        , check for the existence of the semaphore
        , allow read and write permissions
        and an initial value of "NUM_CHILDREN"
    */
	sem_t* children = sem_open(CHILDREN_NAME, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR, NUM_CHILDREN);

	/*
		"The semaphore is destroyed once all other processes that have the semaphore open
       	close it." - https://man7.org/linux/man-pages/man3/sem_unlink.3.html

		i.e. immediately issue the semaphore to be destroyed once all processes have closed it
	*/
	sem_unlink(CHILDREN_NAME);

	return children; // return the semaphore
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

// spawn a child process to handle the integration
void spawn_child_process(sem_t* children, double range_start, double range_end, size_t num_steps, size_t func_id) {
	if (fork()) // ensure we are the child process
		return;

    worker_t workers[NUM_THREADS]; // define an array of workers (one for each thread)
	
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // create a mutex to protect writing to the total area

	spawn_child_threads(workers, &lock, range_start, range_end, num_steps, func_id); // spawn the child threads

	sem_post(children); // increment the children semaphore to allow another process to be spawned
	sem_close(children); // close the child processes access to the semaphore

	exit(EXIT_SUCCESS); // finished
}

// asynchronously wait for a child process to exit
void async_child_wait(int signum) {
	signal(SIGCHLD, async_child_wait); // re-intantiate the signal

	wait(NULL); // acknowledge/wait for the child's death

	num_children--; // notify main loop that a child has been completely removed
}
    
// entry point of the program
int main(void) {
    // define variables populated by stdin
	double range_start, range_end;
	size_t num_steps, func_id;

	sem_t* children = init_named_sem(); // create the named semaphore

	signal(SIGCHLD, async_child_wait); // create a signal to listen for child process's exiting

    // wait for a child process to be available
	while (!sem_wait(children)) {
		while (num_children == NUM_CHILDREN); // a VERY short loop to ENSURE a child process has COMPLETELY exited

		if (!get_valid_input(&range_start, &range_end, &num_steps, &func_id)) // check for valid input
			break;

		num_children++;
		spawn_child_process(children, range_start, range_end, num_steps, func_id); // spawn a child process
	}

	while (wait(NULL) > 0); // (safety) wait for all child processes to finish (if any)

	sem_close(children); // close parent process's access to the named semaphore, destroying it

	exit(EXIT_SUCCESS); // finished
}