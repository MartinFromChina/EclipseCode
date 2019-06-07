#include "..\CommonSource\CharStringDebug\CharStringDebugModule.h"
#include "..\CommonSource\IrqAndTimer\user_irq.h"

#include "user_timer.h"

#define COUNTER_LIMIT  3
//STRING_DEBUG_COUNTER_ENTRY_DEF(p_counter);
STRING_DEBUG_ONCE_ENTRY_DEF(p_entry,0);


static void PrintfInit(void)
{
	setbuf(stdout,NULL);
}

int main(void)
{
	StringDebugInit(PrintfInit);
	UserIrqCreated();
	AllTimerConfig();
	StartTimer();// important !!!

	String_Debug_Once(ENTRY_DEBUG,p_entry,1,(30,"entry 1\r\n"));
	while(1)
	{
		sleep_ms(1);// important !!!
		StringDebugProcess();
	}
	 UserIrqDelete();
	 return 0;
}
