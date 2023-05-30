#include "utils.h"

#include <math.h>
#include <time.h>

double degree_to_radian(double degree)
{
	return degree * M_PI / 180.0;
}

float closer_angle(float angle_to_compare, float option1, float option2)
{
	float diff1, diff2;
	diff1 = fabs(option1 - angle_to_compare);
	diff2 = fabs(option2 - angle_to_compare);

	return (diff1 < diff2 ? option1 : option2);
}

void randperm(int* rand_nrs, int min, int max, int count)
{
	int s, i, interval_length, index_to_swap;
	srand(time(0));
	if (min > max)
	{
		s = max;
		max = min;
		min = s;
	}
	
	interval_length = max - min + 1;
	int all_rands[interval_length];
	
	for (i = min; i <= max; i++)
	{
		all_rands[i] = i;
	}
	for (i = 0; i < interval_length; i++)
	{
		index_to_swap = rand() % (interval_length) + min;
		s = all_rands[i];
		all_rands[i] = all_rands[index_to_swap];
		all_rands[index_to_swap] = s;
	}
	for (i = 0; i < count; i++)
	{
		rand_nrs[i] = all_rands[i];
	}
}
