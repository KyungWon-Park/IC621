#include <stdio.h>
#include <stdlib.h>

void TSP_readmap(int map_data[][15])
{

	// WARNING: FOPEN #0
	FILE *map = fopen("./file-2.txt", "r");
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			fscanf(map, "%d", &map_data[i][j]);
		}
	}
	fclose(map);

	return;
}
