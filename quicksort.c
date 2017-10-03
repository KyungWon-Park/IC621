// Quicksort 
#include <time.h>
#include <stdlib.h>
#include <assert.h>

void QuickSort_IntArr(int *arr, int size)
{
	// Random number generator
	srand((unsigned) time(NULL));

	// Stack initialization
	int *stack = malloc(sizeof(int) * size);
	assert(stack != NULL);
	int stack_ptr = -1;
	
	// Push initial values; Sort arr from arr[0] to arr[size - 1]
	stack[++stack_ptr] = 0;
	stack[++stack_ptr] = (size - 1);
	
	// Internal variables
	int high, low, pivot;
	int partition_index;
	int tmp;
	int i;

	// Work till the stack is empty
	while (stack_ptr > 0)
	{
		// Pop high and low index from the stack
		high = stack[stack_ptr--];
		low  = stack[stack_ptr--];

		// Set the pivot and partition the array
		pivot = rand() % (high - low + 1);
		partition_index = low - 1;
		for (i = low; i <= high - 1; i++)
		{
			if (arr[i] <= pivot)
			{
				partition_index++;
				tmp = arr[partition_index];
				arr[partition_index] = arr[i];
				arr[partition_index] = tmp;
			}
		}
		tmp = arr[partition_index + 1];
		arr[partition_index + 1] = arr[high];
		arr[high] = tmp;
		pivot = partition_index + 1;

		// Check left side of the pivot, push them if necessary
		if (pivot - 1 > low)
		{
			stack[++stack_ptr] = low;
			stack[++stack_ptr] = pivot - 1;
		}
		
		// Check right side of the pivot, push them if necessary
		if (pivot + 1 < high)
		{
			stack[++stack_ptr] = pivot + 1;
			stack[++stack_ptr] = high;
		}
	}

	// Free stack memory and return
	free(stack);
	return;
}
