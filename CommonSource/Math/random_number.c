#include "random_number.h"

#include <stdlib.h>
#include <time.h>

int GetRandomNumber(uint32_t min,uint32_t max)
{
	int tem;
	uint32_t mul,base;

	if(min == max) {return min;}

	if(min < max)
	{
		mul = max - min + 1;
		base = min;
	}
	else
	{
		mul = min - max + 1;
		base = max;
	}

    srand((unsigned)time(NULL));
    tem = rand()%mul + base;

    return tem;
}
