#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define NUM_FUNCS 3
#define NUM_THREADS 4

double gaussian(double);
double charge_decay(double);

typedef double math_func_t(double);
static math_func_t* const funcs[NUM_FUNCS] = {&sin, &gaussian, &charge_decay};

typedef struct {
	double* area;
	pthread_mutex_t* lock;

	double range_start, range_end;
	size_t num_steps, func_id;
} worker_t;

double gaussian(double x) {
	return exp(-(x * x) / 2) / (sqrt(2 * M_PI));
}

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
	worker_t* worker = (worker_t*)arg;

	double range_size = worker->range_end - worker->range_start;
	double dx = range_size / worker->num_steps;

	math_func_t* func = funcs[worker->func_id];

	double area = 0;
	for (size_t i = 0; i < worker->num_steps; i++) {
		double smallx = worker->range_start + i * dx;
		double bigx = worker->range_start + (i + 1) * dx;

		area += func(smallx) + func(bigx);
	}

	area = dx * area / 2;

	pthread_mutex_lock(worker->lock);
	*worker->area += area;
	pthread_mutex_unlock(worker->lock);

	return NULL;
}

bool get_valid_input(double* start, double* end, size_t* num_steps, size_t* func_id) {
	printf("Query: [start] [end] [num_steps] [func_id]\n");

	size_t num_read = scanf("%lf %lf %zu %zu", start, end, num_steps, func_id);

	return (num_read == 4 && *end >= *start && *num_steps > 0 && *func_id < NUM_FUNCS);
}

void spawn_child_threads(worker_t workers[], pthread_mutex_t* lock, double range_start, double range_end, size_t num_steps, size_t func_id) {
	double area = 0;
		
	double dx = (range_end - range_start) / NUM_THREADS;
	size_t steps_per_thread = num_steps / NUM_THREADS;

	pthread_t thread_ids[NUM_THREADS];

	for (size_t i = 0; i < NUM_THREADS; i++) {
		worker_t* worker = &workers[i];

		worker->area = &area;
		worker->lock = lock;
		worker->range_start = range_start + (i * dx);
		worker->range_end = range_start + (i + 1) * dx;
		worker->num_steps = steps_per_thread;
		worker->func_id = func_id;

		pthread_create(&thread_ids[i], NULL, integrate_trap, worker);
	}

	for (size_t i = 0; i < NUM_THREADS; i++)
		pthread_join(thread_ids[i], NULL);

	printf("The integral of function %zu in range %g to %g is %.10g\n", 
		func_id, range_start, range_end, area);
}

int main(void) {
	double range_start, range_end;
	size_t num_steps, func_id;

	worker_t workers[NUM_THREADS];
	
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

	while (get_valid_input(&range_start, &range_end, &num_steps, &func_id))
		spawn_child_threads(workers, &lock, range_start, range_end, num_steps, func_id);

	exit(EXIT_SUCCESS);
}