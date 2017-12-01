#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <pthread.h>
#include <assert.h>
#include "common.h"

int main(int argc, char *argv[])
{
	// Declaration
	__tour__ best_tour;   // Best tour so far
	__tour__ curr_tour;        // Current tour
	// WARNING: MALLOC 0
	__stack__ *p_stack = malloc(sizeof(__stack__) + sizeof(__tour__) * (((NUM_OF_CITY * NUM_OF_CITY) + NUM_OF_CITY) / 2));
	assert(p_stack != NULL && "MALLOC FAILED!\n");

	// Initialization
	// Best tour not found yet
	best_tour.hops = NUM_OF_CITY + 1;
	// best_tour.travel_dist = greedyDistance();
	srand((unsigned int) time(NULL));
	best_tour.travel_dist = initDist();

	// Set up current tour. Only have visited city 0
	curr_tour.hops = 0;
	curr_tour.history[curr_tour.hops] = 0;
	curr_tour.travel_dist = 0;

	// Initialize stack, push curr_tour into it
	(*p_stack).bp = -1;		// bp tells, from this index below, stack pile is empty(or invalid data)
	const int COPYSIZE = sizeof(__tour__);
	memcpy(&(*p_stack).pile[(*p_stack).sp], &curr_tour, COPYSIZE);
	(*p_stack).sp = 0;		// sp telss, this index is the last valid data which can be poped from stack pile

	// Begin stopwatch
	clock_t time_begin = clock();

	while ((*p_stack).sp >=0)
	{	// Only while stack is not empty
		// Pop one "tour" from stack
		curr_tour = (*p_stack).pile[(*p_stack).sp];
		(*p_stack).sp--;

		if (curr_tour.hops == NUM_OF_CITY - 1)
		{	// Then next dest is hometown
			int total_dist = curr_tour.travel_dist + MAP[curr_tour.history[NUM_OF_CITY - 1]][0];
			if ( total_dist < best_tour.travel_dist)
			{	// curr_tour is superior than best_tour found so far. So let's update best tour to curr tour
				memcpy(best_tour.history, curr_tour.history, (sizeof(int) * (NUM_OF_CITY)));
				best_tour.travel_dist = total_dist;
			}
		}
		else
		{	// Still far away to go
			for (int nbr = NUM_OF_CITY - 1; nbr >= 1; nbr--)
			{	// Iterate over all neighbors. Do it in backward to keep order of traversal
				int ivebeenthere = 0;
				for (int i = 0; i <= curr_tour.hops; i++)
				{	// Check if already visited
					if (nbr == curr_tour.history[i])
					{
						ivebeenthere = 1;
						break;
					}
				}

				if (!ivebeenthere)
				{	// Never been there
					// Set this neighbor to next destination
					int fromheretothere = MAP[curr_tour.history[curr_tour.hops]][nbr];
					if (curr_tour.travel_dist + fromheretothere <= best_tour.travel_dist)
					{	// Pruning. Visit only if it still has chance to be the best tour
						curr_tour.hops++;
						curr_tour.history[curr_tour.hops] = nbr;
						curr_tour.travel_dist += fromheretothere;

						// Now push this travel plan to stack
						(*p_stack).sp++;
						memcpy(&(*p_stack).pile[(*p_stack).sp], &curr_tour, COPYSIZE);

#ifdef DEBUG
						printf("Current tour: ");
						for (int j = 0; j <= curr_tour.hops; j++)
						{
							printf("%d -> ", curr_tour.history[j]);
						}
						printf("0\n");
#endif

						// Now roll back curr_tour to previous state since we have pushed it to stack
						curr_tour.travel_dist -= fromheretothere;
						curr_tour.hops--;
					}
				}
			}
		}
	}

	// WARNING: FREE 0
	free(p_stack);

	// Stop stopwatch
	clock_t time_end = clock();
	double time_spent = (double)(time_end - time_begin) / CLOCKS_PER_SEC;

	printf("\n --------------- FINISHED ------------------- \n\n");
	printf("Elapsed time: %lf secs\n", time_spent);
	printf("Best tour distance: %d\n", best_tour.travel_dist);
	printf("Best tour history: ");
	for (int i = 0; i <= NUM_OF_CITY - 1; i++)
	{
		printf("%d -> ", best_tour.history[i]);
	}
	printf("0\n");

	return 0;
}
