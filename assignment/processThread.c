#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <poll.h>
#include <errno.h>
#include <pthread.h>
#include <sys/wait.h>

#define NUM_FUNCS 3	   // number of math functions
#define NUM_CHILDREN 4 // number of children that can be created
#define NUM_THREADS 4  // number of threads that can be created

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
	double *area;
	pthread_mutex_t *lock;

	double range_start, dx;
	size_t start_step, incr_step;
	size_t num_steps, func_id;
} worker_t;

// decalre a pipe to "self-pipe" to parent process
static int self_pipe_fds[2] = {0};

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

	double area = 0;																   // the area of the integral
	for (size_t i = worker->start_step; i < worker->num_steps; i += worker->incr_step) // iterate over all steps in the integral
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
void spawn_child_threads(worker_t workers[], pthread_mutex_t *lock, double range_start, double range_end, size_t num_steps, size_t func_id)
{
	double area = 0; // the total area

	double dx = (range_end - range_start) / num_steps; // the "step" size of each sub-integral

	pthread_t thread_ids[NUM_THREADS]; // store the thread ids

	// create a worker thread
	for (size_t i = 0; i < NUM_THREADS; i++)
	{
		worker_t *worker = &workers[i];

		worker->area = &area;
		worker->lock = lock;
		worker->range_start = range_start;
		worker->dx = dx;
		worker->start_step = i;
		worker->incr_step = NUM_THREADS;
		worker->num_steps = num_steps;
		worker->func_id = func_id;

		if (pthread_create(&thread_ids[i], NULL, integrate_trap, worker) != 0) // spawn a thread in the integrate_trap with the worker initiated above
			error_exit("pthread_create");
	}

	// wait for all the threads to finish
	for (size_t i = 0; i < NUM_THREADS; i++)
		if (pthread_join(thread_ids[i], NULL) != 0)
			error_exit("pthread_join");

	// print the result
	if (printf("The integral of function %zu in range %g to %g is %.10g\n", func_id, range_start, range_end, area) < 0)
		error_exit("printf");
}

// spawn a child process to handle the integration
void spawn_child_process(double range_start, double range_end, size_t num_steps, size_t func_id)
{
	int pid = fork(); // create a child process
	if (pid == -1)
		perror("fork");
	else if (pid != 0) // parent process does not belong here
		return;

	// close the pipes in the child processes as theyre only for the parent
	if (close(self_pipe_fds[0]) == -1)
		error_exit("close");
	if (close(self_pipe_fds[1]) == -1)
		error_exit("close");

	worker_t workers[NUM_THREADS]; // define an array of workers (one for each thread)

	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // create a mutex to protect writing to the total area

	spawn_child_threads(workers, &lock, range_start, range_end, num_steps, func_id); // create threads in the child process to handle computation

	exit(EXIT_SUCCESS); // finished
}

// asynchronously wait for a child process to exit
void async_child_wait(int signum)
{
	signal(SIGCHLD, async_child_wait); // re-intantiate the signal

	wait(NULL); // acknowledge/wait for the child's death

	// write one byte to the same process (non-blocking)
	write(self_pipe_fds[1], ".", 1); // we cant really error check this
}

// created pipes to communicate between signal handler and poll
void init_self_poll(void)
{
	if (pipe(self_pipe_fds) == -1) // create a pipe
		error_exit("pipe");

	if (fcntl(self_pipe_fds[0], F_SETFL, O_NONBLOCK) == -1) // set the output end as non-blocking
		error_exit("fcntl_setfl");

	if (fcntl(self_pipe_fds[1], F_SETFL, O_NONBLOCK) == -1) // set the input end as non-blocking
		error_exit("fcntl_setfl");

	char data[NUM_CHILDREN] = {0};						   // create a byte array for NUM_CHILDREN bytes
	if (write(self_pipe_fds[1], data, NUM_CHILDREN) == -1) // fill pipe with NUM_CHILDREN bytes
		error_exit("write");
}

// uses polling to wait for a process to be available
bool self_poll(void)
{
	// create a poll type variable to read from a fd
	struct pollfd pollfd = {0};
	pollfd.fd = self_pipe_fds[0];
	pollfd.events = POLLIN;

	// store the return value of the poll
	int poll_ret;
	while ((poll_ret = poll(&pollfd, 1, -1)) == -1 && errno == EINTR) // continue polling if poll was interrupted
		;

	if (poll_ret == -1) // if something else went wrong
		error_exit("poll");

	// read the byte from the buffer (non-blocking)
	char discard;
	if (read(pollfd.fd, &discard, 1) == -1)
		error_exit("read");

	return true; // finished
}

// entry point of the program
int main(void)
{
	// define variables populated by stdin
	double range_start, range_end;
	size_t num_steps, func_id;

	init_self_poll(); // init file descriptors for self-polling

	if (signal(SIGCHLD, async_child_wait) == SIG_ERR) // create a signal to listen for child process's exiting
		error_exit("signal");

	// wait for a child process to be available and check for valid input
	while (self_poll() && get_valid_input(&range_start, &range_end, &num_steps, &func_id))
		spawn_child_process(range_start, range_end, num_steps, func_id); // spawn a child process

	while (wait(NULL) > 0) // (safety) wait for all child processes to finish (if any)
		;

	// close the pipes
	if (close(self_pipe_fds[0]) == -1)
		error_exit("close");
	if (close(self_pipe_fds[1]) == -1)
		error_exit("close");

	exit(EXIT_SUCCESS); // finished
}