#include "Button.h"
#include "ButtonTB.h"
#include "..\..\..\CommonSource\AppCommon.h"

RTT_DEBUG_ONCE_ENTRY_DEF(p_button,0);

FILE * ButtonTestOpenFile(void)
{
	return fopen(".//TB_TinyDancer//Button//command.txt", "r");
}
X_Void onTick(X_Void)
{
	static uint8_t value;
	value = GetCurrentButtonValue();
	SEGGER_RTT_Debug_Once(BUTTON_SCRIPT_DEBUG,p_button,value,(40,"button : %s ,%d\r\n",ValueToBinaryString(X_True,16,value),value));
}
