#include "PowerSaving.h"
#include "..\ScriptsCommandModule\HexCommand.h"
#include "..\..\CommonSource\Math\random_number.h"
#include "..\..\CommonSource\CharStringDebug\CharStringDebugModule.h"

STRING_DEBUG_ONCE_ENTRY_DEF(p_move_entry,0);
STRING_DEBUG_ONCE_ENTRY_DEF(p_ble_entry,0);
STRING_DEBUG_ONCE_ENTRY_DEF(p_magnetic_entry,0);

#define SCRIPT_FUNCTION_DEBUG		1

static X_Boolean ScriptsFunctionInitial(X_Void * p_param)
{
	uint8_t i;
	String_Debug(SCRIPT_FUNCTION_DEBUG,(30,"1 :initial is called\r\n"));

	ScriptFunctionParam * p_SFP;
	p_SFP = (ScriptFunctionParam *)p_param;

	p_SFP->isParamValid = X_False;
	p_SFP->recent_pass_function_number = 0;
	p_SFP->param_from_script = 0;

	for(i=0;i<ScriptFunctionParamBufLength;i++)
	{
		p_SFP->buf_8[i]		= 0;
		p_SFP->buf_16[i]	= 0;
		p_SFP->buf_32[i]	= 0;
	}

	return X_True;
}
static X_Boolean ForNow(X_Void * p_param)
{
	ScriptFunctionParam * p_SFP;
	p_SFP = (ScriptFunctionParam *)p_param;

	p_SFP ->isParamValid = X_True;
	p_SFP ->buf_8[0] = 0;
	p_SFP ->buf_8[1] = p_SFP->param_from_script;
	String_Debug(SCRIPT_FUNCTION_DEBUG,(30,"2 : ForNow is called : %d\r\n",p_SFP ->buf_8[1]));
	return X_True;
}
static X_Boolean LoopSpecificTimes(X_Void * p_param)
{
	ScriptFunctionParam * p_SFP;
	p_SFP = (ScriptFunctionParam *)p_param;

//	String_Debug(SCRIPT_FUNCTION_DEBUG,(30,"3 : Loop is called : %d\r\n",p_SFP ->buf_8[0]));

	p_SFP ->isParamValid = X_True;
	if(p_SFP ->buf_8[5]+1 >= p_SFP ->buf_8[6])
	{
		p_SFP ->buf_8[5] = 0;
		return X_True;
	}
	else
	{
		p_SFP ->buf_8[5] ++;
		return X_False;
	}
}
static X_Boolean StateGenerator(X_Void * p_param)
{
	uint32_t random_num;

	ScriptFunctionParam * p_SFP;
	p_SFP = (ScriptFunctionParam *)p_param;

	p_SFP->buf_8[5] = 0;
	p_SFP->buf_8[6] = p_SFP ->param_from_script;

	random_num = GetRandomNumber(1,20);
	String_Debug(SCRIPT_FUNCTION_DEBUG,(30,"4 : random_num %d\r\n",random_num));

	if(random_num == 20)
	{
		String_Debug(SCRIPT_FUNCTION_DEBUG,(30,"user shut down the pen\r\n"));
	}
	if(random_num % 5 == 0)
	{
		String_Debug_Once(SCRIPT_FUNCTION_DEBUG,p_move_entry,1,(30,"pen silence\r\n"));
	}
	else if(random_num % 4 == 0)
	{
		String_Debug_Once(SCRIPT_FUNCTION_DEBUG,p_move_entry,2,(30,"pen move\r\n"));
	}

	if(random_num % 2 == 0)
	{
		String_Debug_Once(SCRIPT_FUNCTION_DEBUG,p_ble_entry,1,(30,"ble connected \r\n"));
	}
	else
	{
		String_Debug_Once(SCRIPT_FUNCTION_DEBUG,p_ble_entry,2,(30,"ble dis connected \r\n"));
	}


	if(p_SFP ->buf_8[0] >= p_SFP ->buf_8[1] )
	{
		p_SFP ->buf_8[0] = 0;
		String_Debug(SCRIPT_FUNCTION_DEBUG,(30,"pen charge state change\r\n"));
	}
	else
	{
		p_SFP ->buf_8[0] = p_SFP ->buf_8[0] + random_num;
	}

	p_SFP ->buf_8[3] = p_SFP ->buf_8[3] + random_num;
	if(p_SFP ->buf_8[3] >= 60 && p_SFP ->buf_8[3] <= 100 )
	{
		String_Debug_Once(SCRIPT_FUNCTION_DEBUG,p_magnetic_entry,1,(30,"pen near magnetic\r\n"));
	}
	else if (p_SFP ->buf_8[3] < 60 )
	{
		String_Debug_Once(SCRIPT_FUNCTION_DEBUG,p_magnetic_entry,2,(30,"pen far away from magnetic\r\n"));
	}
	else
	{
		p_SFP ->buf_8[3] = 0;
	}


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
	  {4,StateGenerator},
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
