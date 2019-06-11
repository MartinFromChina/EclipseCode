#include "CalledByScripts.h"
#include "..\CommonSource\CharStringDebug\CharStringDebugModule.h"

#define SCRIPT_FUNCTION_DEBUG		1

#define SPECIFIC_LOOP_TIMES        5
static uint16_t loop_counter;
static X_Boolean ScriptsFunctionInitial(X_Void * p_param)
{
	String_Debug(SCRIPT_FUNCTION_DEBUG,(30,"1 :initial is called\r\n"));
	loop_counter = 0;
	return X_True;
}
static X_Boolean ForNow(X_Void * p_param)
{
	String_Debug(SCRIPT_FUNCTION_DEBUG,(30,"2 : ForNow is called\r\n"));
	return X_True;
}
static X_Boolean LoopSpecificTimes(X_Void * p_param)
{
	String_Debug(SCRIPT_FUNCTION_DEBUG,(30,"3 : Loop is called\r\n"));
	if(loop_counter >= (SPECIFIC_LOOP_TIMES - 1))
	{
		loop_counter = 0;
		return X_False;
	}
	else
	{
		loop_counter ++;
		return X_True;
	}
}
static X_Boolean Function4(X_Void * p_param)
{
	String_Debug(SCRIPT_FUNCTION_DEBUG,(30,"4 : Function4 is called\r\n"));
	return X_False;
}

static struct _ScriptsFunctionArray
{
	uint8_t function_number;
	X_Boolean(*action)(X_Void * p_param);
}
const ScriptsFunctionArray[] = {
	  {1,ScriptsFunctionInitial},
	  {2,ForNow},
	  {3,LoopSpecificTimes},
	  {4,Function4},
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
		}
	}
	return isOK;
}
