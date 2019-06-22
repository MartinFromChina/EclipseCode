#include "CalledByScripts.h"
#include "HexCommand.h"
#include "..\CommonSource\CharStringDebug\CharStringDebugModule.h"

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

	String_Debug(SCRIPT_FUNCTION_DEBUG,(30,"3 : Loop is called : %d\r\n",p_SFP ->buf_8[0]));

	p_SFP ->isParamValid = X_True;
	if(p_SFP ->buf_8[0]+1 >= p_SFP ->buf_8[1])
	{
		p_SFP ->buf_8[0] = 0;
		return X_True;
	}
	else
	{
		p_SFP ->buf_8[0] ++;
		return X_False;
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
			break;
		}
	}
	return isOK;
}
