// Monte Carlo PI calculator

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <assert.h>

typedef struct 
{
	long long int local_num_toss;
	double local_pi[0];
} __toss_common__;

typedef struct 
{
	int rank;
	__toss_common__ *ptr_common;
} __toss_args__;

void *toss(void *toss_args)
{
	__toss_args__ *ptr_toss_arg = (__toss_args__ *) toss_args; 
	long long int local_num_toss = (*(*ptr_toss_arg).ptr_common).local_num_toss;
	long long int num_in_circle = 0;
	long long int i;
	int pct = 0;
	double x;
	double y;
	double local_pi;
	printf("Rank %d local num toss: %lld\n", (*ptr_toss_arg).rank, local_num_toss);
	for (i = 0; i < local_num_toss; i++)
	{
		x = (rand()) / ((double) RAND_MAX);	
		y = (rand()) / ((double) RAND_MAX);	
		if ((x * x + y * y) <= 1)
		{	num_in_circle++;	}

		if (i >= (local_num_toss / 100) * (pct + 1))
		{
			pct++;
			printf("Rank %d tossed %d Percent\n", (*ptr_toss_arg).rank, pct);
		}
	}

	local_pi = 4 * num_in_circle / ((double) local_num_toss);
	(*(*ptr_toss_arg).ptr_common).local_pi[(*ptr_toss_arg).rank] = local_pi;
	return NULL;
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Error! requires 2 arguments! (num of threads, num of tosses (Billion))\n");
		return -1;
	}

	const int num_threads = atoi(argv[1]);
	const int tmp_num_toss = atoi(argv[1]);

	// WARNING: MALLOC 0
	__toss_common__ *ptr_toss_common = malloc(sizeof(__toss_common__) + (sizeof(double) * num_threads));
	assert(ptr_toss_common != NULL && "malloc failed!\n");

	long long int global_num_toss = (long long int) ((((long long int) tmp_num_toss) * 1024 * 1024 * 1024) / (long long int) num_threads);
	printf("Global num toss: %lld\n", global_num_toss);
	(*ptr_toss_common).local_num_toss =  (long long int) (global_num_toss / (long long int) num_threads);
	printf("Local num toss: %lld\n", (*ptr_toss_common).local_num_toss);

	long thread;
	pthread_t *thread_handles;
	// WARNING: MALLOC 1
	thread_handles = malloc(sizeof(pthread_t) * num_threads);
	assert(thread_handles != NULL && "malloc failed!\n");

	srand((unsigned int) time(NULL));
	__toss_args__ toss_args[num_threads];
	for (thread = 0; thread < num_threads; thread++)
	{
		toss_args[thread].rank = thread;
		toss_args[thread].ptr_common = ptr_toss_common;
		pthread_create(&thread_handles[thread], NULL, toss, (void *) &toss_args[thread]);
		printf("Thread %ld has been created\n", thread);
	}

	for (thread = 0; thread < num_threads; thread++)
	{
		pthread_join(thread_handles[thread], NULL);
		printf("Thread %ld has been joined\n", thread);
	}
	
	double PI_estimate = 0;
	for (thread = 0; thread < num_threads; thread++)
	{
		PI_estimate += (*ptr_toss_common).local_pi[thread];
		printf("PI_estimate: %lf\n", PI_estimate);
	}
	PI_estimate = PI_estimate / num_threads;

	printf("Num threads: %d, Num tosses: %d B, Pi estimate: %lf\n", num_threads, tmp_num_toss, PI_estimate);

	free(ptr_toss_common);
	free(thread_handles);
	return 0;
}
