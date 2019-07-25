#include "..\CommonSource\CharStringDebug\CharStringDebugModule.h"
#include "..\CommonSource\IrqAndTimer\user_irq.h"
#include "..\CommonSource\FileOperation\read_scripts.h"

#include "user_timer.h"

#define COUNTER_LIMIT  3
//STRING_DEBUG_COUNTER_ENTRY_DEF(p_counter,0);
//STRING_DEBUG_ONCE_ENTRY_DEF(p_entry,0);


static void PrintfInit(void)
{
	setbuf(stdout,NULL);
}

static FILE * UserOpenFile(void)
{
	return fopen(".//ScriptsCommandModule//command.txt", "r");
}

int main(void)
{
	StringDebugInit(PrintfInit);

	ReadScriptsInit(UserOpenFile);

	UserIrqCreated();
	AppTimerModuleStart(AllTimerConfig);
	StartTimer();// important !!!

//	String_Debug_Once(ENTRY_DEBUG,p_entry,1,(30,"entry 1\r\n"));
	while(1)
	{
		sleep_ms(500);// important !!!
	}
	 UserIrqDelete();
	 return 0;
}
