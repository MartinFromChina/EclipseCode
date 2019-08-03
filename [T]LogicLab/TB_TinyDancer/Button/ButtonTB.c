#include "ButtonTB.h"
#include "Button.h"
//#include "..\..\..\CommonSource\StateMachine\StateMachine.h"
#include "..\..\ScriptsCommandModule\HexCommand.h"
#include "..\..\..\CommonSource\Math\random_number.h"

RTT_DEBUG_ONCE_ENTRY_DEF(p_script_func_num,100);

static uint8_t button_value = 0;

/***********
 * 0755xxccaabbpp	   :	call number xx function
 * 					cc : 00   pointer to next line
 * 					cc : 01   if xx function return true , pointer to aa line ; else pointer to bb line
 *					pp : parameter to number xx function
 */
/*
 *	1	:FunctionInitial					(1)  00010100
 *	2	:EventGenerator						(2)  01020100
 *
1:command:07550100010100
2:command:07550201020100
3:end
 */

static X_Boolean ScriptsFunctionInitial(X_Void * p_param)
{
	SEGGER_RTT_Debug_Once(BUTTON_SCRIPT_DEBUG,p_script_func_num,1,(30,"--ScriptsFunctionInitial \r\n"));
	button_value = 0;
	ButtonInitialize();
	return X_True;
}
static X_Boolean EventGenerator(X_Void * p_param)
{
	SEGGER_RTT_Debug_Once(BUTTON_SCRIPT_DEBUG,p_script_func_num,2,(30,"EventGenerator \r\n"));
	button_value = GetRandomNumber(0,MAX_BUTTON_VALUE);
	return X_True;

}

static struct _ScriptsFunctionArray
{
	uint8_t function_number;
	X_Boolean(*action)(X_Void * p_param);
}
const ScriptsFunctionArray[] = {
	  {1,ScriptsFunctionInitial},
	  {2,EventGenerator},
};

X_Boolean CallFunction(uint8_t func_num,X_Void * p_param)
{
	uint8_t i;
	X_Boolean isOK;

	isOK = X_False;
	for(i=0;i<sizeof(ScriptsFunctionArray)/sizeof(ScriptsFunctionArray[0]);i++)
	{
		if(ScriptsFunctionArray[i].function_number == func_num && ScriptsFunctionArray[i].action != X_Null)
		{
			isOK = ScriptsFunctionArray[i].action(p_param);
			break;
		}
	}
	return isOK;
}

uint32_t  GetCurrentButtonValue(X_Void)
{
	return button_value;
}
