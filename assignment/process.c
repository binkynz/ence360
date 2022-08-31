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

#define NUM_FUNCS 3
#define NUM_CHILDREN 4
#define CHILDREN_NAME "/children_sem"

double gaussian(double);
double charge_decay(double);

typedef double math_func_t(double);
static math_func_t* const funcs[NUM_FUNCS] = {&sin, &gaussian, &charge_decay};

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
double integrate_trap(math_func_t* func, double range_start, double range_end, size_t num_steps) {
	double range_size = range_end - range_start;
	double dx = range_size / num_steps;

	double area = 0;
	for (size_t i = 0; i < num_steps; i++) {
		double smallx = range_start + i*dx;
		double bigx = range_start + (i+1)*dx;

		area += func(smallx) + func(bigx);
	}

	area = dx * area / 2;

	return area;
}

bool get_valid_input(double* start, double* end, size_t* num_steps, size_t* func_id) {
	printf("Query: [start] [end] [num_steps] [func_id]\n");

	size_t num_read = scanf("%lf %lf %zu %zu", start, end, num_steps, func_id);

	return (num_read == 4 && *end >= *start && *num_steps > 0 && *func_id < NUM_FUNCS);
}

sem_t* init_named_sem(void) {
	sem_t* children = sem_open(CHILDREN_NAME, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR, NUM_CHILDREN);

	/*
		"The semaphore is destroyed once all other processes that have the semaphore open
       	close it." - https://man7.org/linux/man-pages/man3/sem_unlink.3.html

		i.e. immediately issue the semaphore to be destroyed once all processes have closed it
	*/
	sem_unlink(CHILDREN_NAME);

	return children;
}

void spawn_child_process(sem_t* children, double range_start, double range_end, size_t num_steps, size_t func_id) {
	if (fork())
		return;

	double area = integrate_trap(funcs[func_id], range_start, range_end, num_steps);

	printf("The integral of function %zu in range %g to %g is %.10g\n", 
		func_id, range_start, range_end, area);

	sem_post(children);
	sem_close(children);

	exit(EXIT_SUCCESS);
}

int main(void) {
	double range_start, range_end;
	size_t num_steps, func_id;

	sem_t* children = init_named_sem();

	while (!sem_wait(children) && get_valid_input(&range_start, &range_end, &num_steps, &func_id))
		spawn_child_process(children, range_start, range_end, num_steps, func_id);

	while (wait(NULL) > 0); // wait for all child processes to finish

	sem_close(children);

	exit(EXIT_SUCCESS);
}