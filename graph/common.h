#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>

#define NUM_OF_CITY 15
#define UNDEFINED -8926974
//#define DEBUG 1

extern int MAP[NUM_OF_CITY][NUM_OF_CITY];

typedef enum {PENDING, WORKING, STOLEN, FINISHED} __state__;

typedef struct 
{
	__state__ state;	
	int hops;
	int travel_dist;
	int history[NUM_OF_CITY];
} __tour__;

typedef struct 
{
	struct __stack__ *addr;
	int bp;
	int sp;
	__tour__ pile[0];
} __stack__;

int greedyDist(void);

int randomDist(void);

int initDist(void);

