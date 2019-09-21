#include "..\CommonSource\CharStringDebug\CharStringDebugModule.h"
#include "..\CommonSource\IrqAndTimer\user_irq.h"
#include "..\CommonSource\FileOperation\read_scripts.h"

#include "user_timer.h"
//#include "TB_TinyDancer\Button\Button.h"
#include "TB_TinyDancer\FlashReadWrite\UserFlash.h"


static void PrintfInit(void)
{
	setbuf(stdout,NULL);
}
/*
static FILE * UserOpenFile(void)
{
	return fopen(".//ScriptsCommandModule//command.txt", "r");
}
*/
int main(void)
{
	StringDebugInit(PrintfInit);

	ReadScriptsInit(FlashTestOpenFile);

	UserIrqCreated();
	AppTimerModuleStart(AllTimerConfig);
	StartTimer();// important !!!

	while(1)
	{
		sleep_ms(500);// important !!!
	}
	 UserIrqDelete();
	 return 0;
}
