#include "HexCommand.h"
//#include "CalledByScripts.h"
#include "..\TB_PowerSaving\PowerSaving.h"
#include "..\..\CommonSource\CharStringDebug\CharStringDebugModule.h"

#define ConditionalCommandLength  7

static ScriptFunctionParam  SFP;

X_Boolean DoAsCommand(uint8_t* p_command,uint8_t length)
{
	String_Debug(LOAD_COMMAND_DEBUG,(30,"command %2x %2x %2x; length:%d \r\n"
									,p_command[0],p_command[1],p_command[2],length));

	return X_True;
}

/***********
 * 0x0655xxyyaabb	:	call number xx function
 * 					yy : 00   pointer to next line
 * 					yy : 01   if xx function return true , pointer to aa line ; else pointer to bb line
 */
uint32_t ConditionalDoAsCommand(uint8_t* p_command,uint8_t length,uint32_t current_command_line)
{
	X_Boolean isConditional,isOK;
	uint32_t next_command_line;

	isConditional = X_False;
	if(p_command[1] == 0x55 && length == ConditionalCommandLength)
	{

		if(p_command[4]< 1 || p_command[5] < 1)
		{
			String_Debug(LOAD_COMMAND_DEBUG,(30,"command %2x %2x %2x; length:%d is wrong \r\n"
												,p_command[0],p_command[1],p_command[2],length));
			return current_command_line;
		}

		if(p_command[3] == 0) {isConditional = X_False;}
		else {isConditional = X_True;}

		SFP.param_from_script = p_command[6];
		isOK = CallFunction(p_command[2],&SFP);

		if(isConditional == X_True)
		{
			if(isOK == X_True)
			{
				next_command_line = p_command[4] - 1;
			}
			else
			{
				next_command_line = p_command[5] - 1;
			}
			return next_command_line;
		}
	}

	return (current_command_line);// +1 is already done by read module
}

