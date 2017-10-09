#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "quicksort.h"
#define NO_PARTNER -24425195

int *randGen_IntArr(int size)
{
	// Remember to free returned arr after business
	srand((unsigned int) time(NULL));
	int *arr = malloc(sizeof(int) * size);
	assert(arr != NULL && "Problem set initialization failed\n");
	int i;
	for (i = 0; i < size; i++)
	{
		arr[i] = rand();
	}
	return arr;
}

int findPartner(int phase, int my_rank, int comm_sz)
{
	int partner;

	if (phase % 2 == 0)
	{
		if (my_rank % 2 == 0)
		{ partner = my_rank + 1; }
		else 
		{ partner = my_rank - 1; }
	}
	else
	{
		if (my_rank % 2 == 0)
		{ partner = my_rank - 1; }
		else 
		{ partner = my_rank + 1; }
	}

	if (partner < 0 || partner >= comm_sz)
	{ partner = NO_PARTNER; }

	return partner;
}


int main(int argc, char *argv[])
{
	// MPI initialization
	int comm_sz = -1;
	int my_rank = -1;
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	assert(!(comm_sz < 0 || my_rank < 0) && "MPI initialization failed\n");
	
	// Tell problem set size
	int problem_set_size;
	// Shell prompt for problem set size
	if (my_rank == 0 && (argc == 1))
	{
		printf("Enter problem set size ( * M integer keys): ");
		scanf("%d", &problem_set_size);
	}
	else 
	{
		problem_set_size = atoi(argv[1]); 
	}

	problem_set_size = problem_set_size * 1024 * 1024;
	MPI_Bcast(&problem_set_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

	char buf[255] = "";
	char output_dir[255] = "";
	char output_path[255] = "";	// For later use;
	// Prepare for output
	if (my_rank == 0)
	{
		sprintf(buf, "%d", comm_sz);
		strcat(output_dir, buf);
		strcpy(buf, "");
		strcat(output_dir, "_nodes_");
		sprintf(buf, "%d", (problem_set_size / (1024 * 1024)));
		strcat(output_dir, buf);
		strcpy(buf, "");
		strcat(output_dir, "_M_keys_Result_");
	}
	
	// Problem set distribution
	int local_set_sz = problem_set_size / comm_sz;
	// WARNING: MALLOC
	int *arr_Mine = malloc(sizeof(int) * local_set_sz);		// Starting arr of every cycle 
	// WARNING: MALLOC
	int *arr_notMine = malloc(sizeof(int) * local_set_sz);	// Array from partner node
	// WARNING: MALLOC
	int *arr_beMine = malloc(sizeof(int) * local_set_sz);	// Array to keep (smaller or bigger) 
	
	assert(arr_Mine != NULL && "Local set malloc failed\n");
	assert(arr_notMine != NULL && "Local set malloc failed\n");
	assert(arr_beMine != NULL && "Local set malloc failed\n");

	int *problem_set;
	if (my_rank == 0)
	{
		// WARNING: MALLOC
		problem_set = randGen_IntArr(problem_set_size);
		// WARNING: FILE OPEN
		strcat(output_path, output_dir);
		strcat(output_path, "problem_set.txt");
		FILE *f = fopen(output_path, "w");
		if (f == NULL)
		{
			printf("Output file: %s\n", output_path);
			printf("Cannot open file for problem set write. Ignoring...\n");
		}
		else
		{
			int i;
			for (i = 0; i < problem_set_size; i++)
			{
				fprintf(f, "%d\n", problem_set[i]);
			}
		}
		fclose(f);
		strcpy(output_path, "");
		printf("\n");
		printf("Problem set size: %d M __AND__ Number of workers: %d\n", problem_set_size / (1024 * 1024), comm_sz);
		printf("Problem set generation complete. Entrancing integer sorting routine...\n");
	}
	double t_start, t_finish, t_elapsed;
	t_start = MPI_Wtime();
	MPI_Scatter(problem_set, local_set_sz, MPI_INT, arr_Mine, local_set_sz, MPI_INT,  0, MPI_COMM_WORLD);

	// Initial local sorting
	QuickSort_IntArr(arr_Mine, local_set_sz);

	// Set up for odd even transposition sorting
	int phase;
	int partner = NO_PARTNER;
	int i;
	int head_Mine, head_notMine;
	int *tmp_ptr;

	// Start odd even transposition sorting
	for (phase = 0; phase < comm_sz; phase++)
	{
		// Sex
		partner = findPartner(phase, my_rank, comm_sz);

		// Synchronization
		MPI_Barrier(MPI_COMM_WORLD);
		if (partner != NO_PARTNER)
		{
			// Exchange arr 
			if (my_rank % 2 == 0)
			{
				MPI_Send(arr_Mine, local_set_sz, MPI_INT, partner, 0, MPI_COMM_WORLD);
				MPI_Recv(arr_notMine, local_set_sz, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
			else 
			{
				MPI_Recv(arr_notMine, local_set_sz, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				MPI_Send(arr_Mine, local_set_sz, MPI_INT, partner, 0, MPI_COMM_WORLD);
			}

			// Keep yours
			if ((my_rank + phase) % 2 == 0)
			{	// Keep Smaller
				head_Mine = head_notMine = 0;
				for (i = 0; i < local_set_sz; i++)
				{
					if (arr_Mine[head_Mine] <= arr_notMine[head_notMine])
					{
						arr_beMine[i] = arr_Mine[head_Mine++];
					}
					else 
					{
						arr_beMine[i] = arr_notMine[head_notMine++];
					}
				}
			}
			else 
			{	// Keep Larger
				head_Mine = head_notMine = (local_set_sz - 1);
				for (i = local_set_sz - 1; i >=0; i--)
				{
					if (arr_Mine[head_Mine] >= arr_notMine[head_notMine])
					{
						arr_beMine[i] = arr_Mine[head_Mine--];
					}
					else 
					{
						arr_beMine[i] = arr_notMine[head_notMine--];
					}
				}
			}
			tmp_ptr = arr_Mine;
			arr_Mine = arr_beMine;
			arr_beMine = tmp_ptr;
		}
	}

	// Gather results
	MPI_Gather(arr_Mine, local_set_sz, MPI_INT, problem_set, local_set_sz, MPI_INT, 0, MPI_COMM_WORLD);
	t_finish = MPI_Wtime();
	t_elapsed = t_finish - t_start;
	double t_bench;
	MPI_Reduce(&t_elapsed, &t_bench, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	
	if (my_rank == 0)
	{
		printf("Sorting has been finished. Elapsed time: %lf sec\n", t_bench);
		printf("\n[------*------*------*------*------*------*------*------]\n");
		// WARNING: FILE OPEN
		strcat(output_path, output_dir);
		strcat(output_path, "sorted.txt");
		FILE *f_out = fopen(output_path, "w");
		if (f_out == NULL)
		{
			printf("Output file: %s\n", output_path);
			printf("Cannot open file for output write. Ignoring...\n");
		}
		else 
		{
			int i;
			for (i = 0; i < problem_set_size; i++)
			{
				fprintf(f_out, "%d\n", problem_set[i]);
			}
			fclose(f_out);

			f_out = fopen("global_time_report.txt", "a");
			fprintf(f_out, "[Problem set size: %d M & Workers: %d]: %lf sec\n", problem_set_size, comm_sz, t_bench);
		}
		fclose(f_out);

		free(problem_set);
	}

	// Clean up
	free(arr_Mine);
	free(arr_notMine);
	free(arr_beMine);
	MPI_Finalize();

	return 0;
}
