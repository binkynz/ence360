#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define NUM_FUNCS 3
#define NUM_CHILD 4

static char num_children = 0;

double gaussian(double);
double charge_decay(double);

typedef double math_func_t(double);
static math_func_t* const funcs[NUM_FUNCS] = {&sin, &gaussian, &charge_decay};

double gaussian(double x) {
	return exp(-(x*x)/2) / (sqrt(2 * M_PI));
}

double charge_decay(double x) {
	if (x < 0) 
		return 0;
	else if (x < 1)
		return 1 - exp(-5*x);
	else
		return exp(-(x-1));
}

// integrate using the trapezoid method. 
double integrate_trap(math_func_t* func, double range_start, double range_end, size_t num_steps) {
	double range_size = range_end - range_start;
	double dx = range_size / num_steps;

	double area = 0;
	for (size_t i = 0; i < num_steps; i++) {
		double smallx = range_start + i*dx;
		double bigx = range_start + (i+1)*dx;

		area += dx * (func(smallx) + func(bigx)) / 2; // multiply area by dx once at the end. 
	}

	return area;
}

bool get_valid_input(double* start, double* end, size_t* num_steps, size_t* func_id) {
	printf("Query: [start] [end] [num_steps] [func_id]\n");

	size_t num_read = scanf("%lf %lf %zu %zu", start, end, num_steps, func_id);

	return (num_read == 4 && *end >= *start && *num_steps > 0 && *func_id < NUM_FUNCS);
}

void async_child_wait(int signum) {
	signal(SIGCHLD, async_child_wait);

    wait(NULL);
	num_children--;

	// printf("DEBUG num children: %d\n", num_children);
}

int main(void) {
	double range_start, range_end;
	size_t num_steps, func_id;

	signal(SIGCHLD, async_child_wait);

	while (1) {
		if (num_children < NUM_CHILD && get_valid_input(&range_start, &range_end, &num_steps, &func_id)) {
			num_children++;

			if (!fork()) {
				double area = integrate_trap(funcs[func_id], range_start, range_end, num_steps);

				printf("The integral of function %zu in range %g to %g is %.10g\n", 
					func_id, range_start, range_end, area);

				exit(0);
			}
		}
	}

	exit(0);
}