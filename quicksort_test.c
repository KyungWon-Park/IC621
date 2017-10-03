#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "quicksort.h"

int main(int argc, char *argv[])
{
	int size = atoi(argv[1]);
	int i;
	int *arr = malloc(sizeof(int) * size);
	assert(arr != NULL);
	
	srand((unsigned int) time(NULL));
	for (i = 0; i < size; i++)
	{
		arr[i] = (rand() % (3 * size));
	}

	FILE *f_in = fopen("input.txt", "w");
	for (i = 0; i < size; i++)
	{
		fprintf(f_in, "%d\n", arr[i]);
	}
	fclose(f_in);

	QuickSort_IntArr(arr, size);

	FILE *f_out = fopen("output.txt", "w");
	for (i = 0; i < size; i++)
	{
		fprintf(f_out, "%d\n", arr[i]);
	}
	fclose(f_out);

	free(arr);

	return 0;
}
