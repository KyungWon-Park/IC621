#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>
#include "common.h"

int MAP[NUM_OF_CITY][NUM_OF_CITY] = {
	{ 0, 83, 86, 77, 15, 93, 35, 86, 92, 49, 21, 62, 27, 90, 59},
	{63,  0, 26, 40, 26, 72, 36, 11, 68, 67, 29, 82, 30, 62, 23},
	{67, 35,  0, 29,  2, 22, 58, 69, 67, 93, 56, 11, 42, 29, 73},
	{21, 19, 84,  0, 37, 98, 24, 15, 70, 13, 26, 91, 80, 56, 73},
	{62, 70, 96, 81,  0,  5, 25, 84, 27, 36,  5, 46, 29, 13, 57},
	{24, 95, 82, 45, 14,  0, 67, 34, 64, 43, 50, 87,  8, 76, 78},
	{88, 84,  3, 51, 54, 99,  0, 32, 60, 76, 68, 39, 12, 26, 86},
	{94, 39, 95, 70, 34, 78, 67,  0,  1, 97,  2, 17, 92, 52, 56},
	{ 1, 80, 86, 41, 65, 89, 44, 19,  0, 40, 29, 31, 17, 97, 71},
	{81, 75,  9, 27, 67, 56, 97, 53, 86,  0, 65,  6, 83, 19, 24},
	{28, 71, 32, 29,  3, 19, 70, 68,  8, 15,  0, 40, 49, 96, 23},
	{18, 45, 46, 51, 21, 55, 79, 88, 64, 28, 41,  0, 50, 93, 99},
	{34, 64, 24, 14, 87, 56, 43, 91, 27, 65, 59, 36,  0, 32, 51},
	{37, 28, 75,  7, 74, 21, 58, 95, 29, 37, 35, 93, 18,  0, 28},
	{43, 11, 28, 29, 76,  4, 43, 63, 13, 38,  6, 40,  4, 18,  0}};

int greedyDist(void)
{
	__tour__ grdy_tour;
	grdy_tour.hops = 0;
	grdy_tour.history[0] = 0;
	grdy_tour.travel_dist = 0;

	for (int idx_hist = 0; idx_hist < NUM_OF_CITY - 1; idx_hist++)
	{	// Select next city one by one, following greedy algorithm
		int next_city;
		int next_city_dist = INT_MAX;
		for (int i = 1; i < NUM_OF_CITY; i++)
		{	// Pick one city i
			bool beenthere = false; 
			for (int j = 0; j <= grdy_tour.hops; j++)
			{	// Check if this city i was visited before
				if (i == grdy_tour.history[j])
				{
					beenthere = true;
					break;
				}
			}

			if (!beenthere) 
			{
				int fromheretothere = MAP[grdy_tour.history[idx_hist]][i];
				if (fromheretothere < next_city_dist)
				{
					next_city_dist = fromheretothere;
					next_city = i;
				}
			}
		}

		grdy_tour.history[idx_hist + 1] = next_city;
		grdy_tour.hops++;
		grdy_tour.travel_dist += next_city_dist;
	}

#ifdef DEBUG
	printf("Greedy tour path: ");
	for (int i = 0; i < NUM_OF_CITY; i++)
	{
		printf("%d -> ", grdy_tour.history[i]);
	}
	printf("%d\n", 0);

	printf("Total greedy tour distance: %d\n", grdy_tour.travel_dist);
	char tmp;
	printf("\nPress Enter to Continue\n");
	scanf("%c", &tmp);
#endif

	return grdy_tour.travel_dist;
}

int randomDist(void)
{	// Returns random tour distance for aggressive pruning
	__tour__ rand_tour;
	rand_tour.hops = 0;
	rand_tour.history[0] = 0;
	rand_tour.travel_dist = 0;

	for (int idx_hist = 1; idx_hist < NUM_OF_CITY; idx_hist++)
	{	// Fill history with random cities
		int next_city = UNDEFINED;
		bool beenthere = false;
		do 
		{	// Do lottery till we fill history with random cities but each city visited only once
			next_city = (rand() % NUM_OF_CITY);
			for (int idx_city = 0; idx_city <= rand_tour.hops; idx_city++)
			{	// Roll dice again if city has been visited before
				if (next_city == rand_tour.history[idx_city])
				{	// This city was visited before
					beenthere = true;
					break;
				}
				else 
				{	// This city wasn't visited
					rand_tour.history[idx_hist] = next_city;
					beenthere = false;
				}
			}
		} while (beenthere);

		rand_tour.hops++;
		rand_tour.travel_dist += MAP[rand_tour.history[idx_hist - 1]][rand_tour.history[idx_hist]];
	}

	int total_dist = rand_tour.travel_dist + MAP[NUM_OF_CITY][0];

#ifdef DEBUG 
	printf("Graph at this random trial: ");
	for (int i = 0; i < NUM_OF_CITY; i++)
	{
		printf("%d -> ", rand_tour.history[i]);
	}
	printf("%d\n", 0);
	printf("Distance at this trial: %d\n", total_dist);
	printf("\n Press Enter to Continue \n");
	char tmp;
	scanf("%c", &tmp);
#endif

	return total_dist;
}

int initDist(void)
{
	int best_dist;
	best_dist = greedyDist();
	for (int i = 0; i < NUM_OF_CITY; i++)
	{
		int rand_dist = randomDist();
		if (rand_dist < best_dist)
		{
			best_dist = rand_dist;
		}
	}

#ifdef DEBUG 
	printf("Best distance so far: %d\n", best_dist);
	printf("Press Enter to continure\n");
	printf("I'll do pruning with this distance info\n");
	char tmp;
	scanf("%c", &tmp);
#endif 

	return best_dist;
}
