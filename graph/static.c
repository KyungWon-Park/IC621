#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <pthread.h>
#include <assert.h>
#include "common.h"
#include <pthread.h>

int n_threads;	// Number of threads
int stack_start_level; // In which level of tree each threads will start?
int stack_size; // Stack size per threads
int COPYSIZE;

void stackSize(void)
{	// Decide size of stack for each threads and judge stack start level (In which floor all threads will start to work?)
	// Must be called after n_threads has been initialized
	const int num_t = n_threads;
	for (int i = 0; (NUM_OF_CITY ^ i) <= num_t; i++)
	{
		if ((NUM_OF_CITY ^ i) == num_t)
		{	// IF NUM_OF_CITY ^ i is equal with number of threads, then level i should be the floor that threads will start to work
			stack_start_level = i;
			break;
		}
		else if (((NUM_OF_CITY ^ (i + 1)) >= num_t))
		{	// IF NUM_OF_CITY ^ i is smaller than number of threads and NUM_OF_CITY ^ (i + 1) is bigger than number of threads, then (i + 1) is the right floor
			stack_start_level = i + 1;
			break;
		}
		// stack_start_level 0 means "0" in job stack
		// stack_start_level 1 means "0 -> 1" or "0 -> 2" or "0 -> 3" ...
		// stack_start_level 2 means "0 -> 1 -> 2" or "0 -> 1 -> 3" ...
	}

	stack_size = ((NUM_OF_CITY - stack_start_level) * (NUM_OF_CITY - stack_start_level - 1) / 2) + ((NUM_OF_CITY ^ stack_start_level) / n_threads);

	return;
}

int factorial(int n)
{	// Returns factorial
	int fact = 1;
	for (int i = 1; i <= n; i++)
	{
		fact = fact * i;
	}
	return fact;
}

int perm(int r)
{	// Returns (NUM_OF_CITY - 1)_P_r
	int ans;
	int n = NUM_OF_CITY - 1; // Because starting city is 0, and we don't need to visit there ever again in the middle of journey
	ans = factorial(n) / factorial(n - r);
	return ans;
}

void perm_list(int r, int *arr_perm)
{	// Returns array of possible permutations, (NUM_OF_CITY -1)_P_r
	for (int i = 0; i < perm(r); i++)
	{	// For number of (NUM_OF_CITY - 1)_P_r times,
	}
	return;
}

void stackInitManager(__stack__ *stack_t)
{	// Create initial stack, and toss them to each thread's stacks
	// Create my stack to work on 
	// WARNTING: MALLOC 0
	__stack__ *my_stack = malloc(sizeof(__stack__) + sizeof(__tour__) * (NUM_OF_CITY ^ stack_start_level));
	assert(my_stack != NULL && "Malloc Failed!\n");
	// Initialize my_stack
	(*my_stack).bp = -1;
	(*my_stack).sp = -1;

	// WARNING: FREE 0
	free(my_stack);
	return;
}

void *local_job_func(void *local_arg)
{
	return NULL;
}

int main(int argc, char *argv[])
{
	// Get number of threads to create
	if (argc == 2)
	{
		n_threads = atoi(argv[1]);
	}
	else 
	{
		n_threads = 1;
	}
	
	// Initialize necessary variables
	COPYSIZE = sizeof(__tour__);

	return 0;
}
