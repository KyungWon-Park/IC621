#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "read.h"

typedef struct 
{
	int hop;
	int history[16];
} tour;

typedef struct 
{
	int sp;
	tour st[0];
} __stack__;

void ST_push(__stack__ stack, tour element);

int main(int argc, char *argv[])
{
	// map_data[i][j] := distance between city i and j
	int map_data[15][15];
	TSP_readmap(map_data);

	int st_sz = 1;
	for (int i = 1; i < 16; i++)
	{
		st_sz = st_sz * i; 
	}

	__stack__ *stack = malloc(sizeof(__stack__) + st_sz * sizeof(tour));
	// WARNING: MALLOC #0
	(*stack).sp = 0;
	(*stack).st[0].history[0] = 0;
	
	


	// WARNING: FREE #0
	free(stack);
	return 0;
}


