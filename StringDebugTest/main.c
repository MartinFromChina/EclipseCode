#include "StringDebugModule.h"

static void NormalDelay(unsigned int counter)
{
	for(; counter > 0; counter --)
	{

	}
}
static void PrintfInit(void)
{
	setbuf(stdout,NULL);
}

#define COUNTER_LIMIT  3
STRING_DEBUG_COUNTER_ENTRY_DEF(p_main_loop,0);
STRING_DEBUG_ONCE_ENTRY_DEF(p_entry,0);

int main(void)
{
	float tem = 0.3;
	uint8_t i = 0;
	StringDebugInit(PrintfInit);
//	printf("float : %f\r\n",tem);
//	String_Debug(FOW_NOW_DEBUG,(1,"float : %f\r\n",tem));
	String_Debug_Once(ENTRY_DEBUG,p_entry,1,(0,"entry 1\r\n"));
	while(1)
	{
//		String_Debug(FOW_NOW_DEBUG,(0,"main loop alive\r\n"));
		printf("i = %d\r\n", i++);
		String_Debug_Counter(COUNTR_DEBUG,p_main_loop,COUNTER_LIMIT,(0,"main loop counter come\r\n"));
		NormalDelay(0x2fffffff);
		StringDebugProcess();

		String_Debug_Once(ENTRY_DEBUG,p_entry,2,(0,"entry 2\r\n"));

	}
}
