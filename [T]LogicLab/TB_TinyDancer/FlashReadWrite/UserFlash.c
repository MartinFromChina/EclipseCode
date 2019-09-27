#include "UserFlash.h"
#include "UserFlashTB.h"
#include "..\..\..\CustomizedModule\FlashReadWrite\FlashEventHandle.h"											\

APP_FLASH_EVENT_HANDLER_DEFINE(p_flash_handler
								,0,0
								,0,0
								,0,0
								,X_Null,X_Null,X_Null,X_Null,X_Null,X_Null
								);

FILE * FlashTestOpenFile(void)
{
	mFlashEventInit(p_flash_handler);
	return fopen(".//TB_TinyDancer//FlashReadWrite//command.txt", "r");
}
static uint16_t debug_counter = 0;
X_Void onTick(X_Void)
{
	uint8_t sector_number;
	if(GetSectorNumber(&sector_number) == X_True)
	{
		SEGGER_RTT_Debug(FLASH_SECTOR_DEBUG,(60,"**%d**flash ready ? %d ; current sector number %d\r\n"
													,debug_counter,DoesFlashReady(),sector_number));
		debug_counter ++;

//		Test_PriorityQueue(sector_number);
	}
	mFlashEventHandlerRun(p_flash_handler);
}
