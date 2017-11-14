#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#define CHUNK 100

enum scheduler {STATIC, DYNAMIC, GUIDED};

int main(int argc, char *argv[])
{
	// Get Input
	char *st_str = "static";
	char *dy_str = "dynamic";
	char *gi_str = "guided";
	
	char *input = argv[1];
	int chunksize;
	enum scheduler mode;

	clock_t start, end;
	double cpu_time_used;

	if (argc < 2)
	{
		printf("Enter one of: { static, dynamic, guided } as a parameter\n");
		return -1;
	}

	if (!strcmp(input, st_str))
	{
		if (0)
		{
			printf("Error\n");
			return -1;
		}
		else 
		{
			mode = STATIC;		
			// chunksize = atoi(argv[2]);
		}
	}
	else if (!strcmp(input, dy_str))
	{
		if (0)
		{
			printf("Error\n");
			return -1;
		}
		else 
		{
			mode = DYNAMIC;
			// chunksize = atoi(argv[2]);
		}
	}
	else if (!strcmp(input, gi_str))
	{
		mode = GUIDED;
	}
	else 
	{
		printf("Error\n");
		return -1;
	}

	// Initialize random integers
	srand((unsigned int) time(NULL));
	// MALLOC: #0
	int *problem_set = malloc(sizeof(int) * 100000);
	assert(problem_set != NULL && "Malloc failure\n");
	for (int i = 0; i < 100000; i++)
	{
		problem_set[i] = (rand() % 1000000000);
	}

	// Write problem set
	// FP OPEN:  #0
	FILE *fp = fopen("./input.txt", "w");
	for (int i = 0; i < 100000; i++)
	{
		fprintf(fp, "%d\n", problem_set[i]);
	}
	// FP CLOSE: #0
	fclose(fp);

	// Odd Even Sorting ...
	start = clock();
	int i;
	int tmp;
	int phase;
	if (mode == STATIC)
	{
		#	pragma omp parallel num_threads(4) default(none) shared(problem_set) private(i, tmp, phase) 
		for (phase = 0; phase < 100000; phase++)
		{
			if (phase % 2 == 0)
			{
				#	pragma omp for schedule(static, CHUNK)
				for (i = 1; i < 100000; i += 2)
				{
					if (problem_set[i - 1] > problem_set[i])
					{
						tmp = problem_set[i - 1];
						problem_set[i - 1] = problem_set[i];
						problem_set[i] = tmp;
					}
				}
			}
			else 
			{
				#	pragma omp for schedule(static, CHUNK)
				for (i = 1; i < 99999; i += 2)
				{
					if (problem_set[i] > problem_set[i + 1])
					{
						tmp = problem_set[i + 1];
						problem_set[i + 1] = problem_set[i];
						problem_set[i] = tmp;
					}
				}
			}
		}
	}
	else if (mode == DYNAMIC)
	{
		#	pragma omp parallel num_threads(4) default(none) shared(problem_set) private(i, tmp, phase) 
		for (phase = 0; phase < 100000; phase++)
		{
			if (phase % 2 == 0)
			{
				#	pragma omp for schedule(static, CHUNK)
				for (i = 1; i < 100000; i += 2)
				{
					if (problem_set[i - 1] > problem_set[i])
					{
						tmp = problem_set[i - 1];
						problem_set[i - 1] = problem_set[i];
						problem_set[i] = tmp;
					}
				}
			}
			else 
			{
				#	pragma omp for schedule(static, CHUNK)
				for (i = 1; i < 99999; i += 2)
				{
					if (problem_set[i] > problem_set[i + 1])
					{
						tmp = problem_set[i + 1];
						problem_set[i + 1] = problem_set[i];
						problem_set[i] = tmp;
					}
				}
			}
		}
	}
	else if (mode == GUIDED)
	{
		#	pragma omp parallel num_threads(4) default(none) shared(problem_set) private(i, tmp, phase) 
		for (phase = 0; phase < 100000; phase++)
		{
			if (phase % 2 == 0)
			{
				#	pragma omp for schedule(static, CHUNK)
				for (i = 1; i < 100000; i += 2)
				{
					if (problem_set[i - 1] > problem_set[i])
					{
						tmp = problem_set[i - 1];
						problem_set[i - 1] = problem_set[i];
						problem_set[i] = tmp;
					}
				}
			}
			else 
			{
				#	pragma omp for schedule(static, CHUNK)
				for (i = 1; i < 99999; i += 2)
				{
					if (problem_set[i] > problem_set[i + 1])
					{
						tmp = problem_set[i + 1];
						problem_set[i + 1] = problem_set[i];
						problem_set[i] = tmp;
					}
				}
			}
		}
	}
	else 
	{
		return -1;
	}
	end = clock();

	// Write sorted set 
	// FP OPEN: #1
	fp = fopen("./output.txt", "w");
	for (int i = 0; i < 100000; i++)
	{
		fprintf(fp, "%d\n", problem_set[i]);
	}
	// FP CLOSE: #1
	
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Elapsed time: %lf sec\n", cpu_time_used);
	
	fclose(fp);
	// FREE:  #0
	free(problem_set);
	return 0;
}
