#include <stdio.h>

static void NormalDelay(unsigned int counter)
{
	for(; counter > 0; counter --)
	{

	}
}

int main(void)
{
	setbuf(stdout,NULL);
	while(1)
	{
		NormalDelay(0x2fffffff);
		printf("hello world\r\n");
	}
}
