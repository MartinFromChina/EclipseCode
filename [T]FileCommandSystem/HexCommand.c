#include "HexCommand.h"
#include "CalledByScripts.h"
#include "..\CommonSource\CharStringDebug\CharStringDebugModule.h"


X_Boolean DoAsCommand(uint8_t* p_command,uint8_t length)
{
	String_Debug(LOAD_COMMAND_DEBUG,(30,"command %2x %2x %2x; length:%d \r\n"
									,p_command[0],p_command[1],p_command[2],length));

	return X_True;
}

/***********
 * 01	:	call number xx function once // 0301xx
 * 02	: 	call number xx function yy times   immetialy //0402xxyy01
 * 02	: 	call number xx function yy times   //0402xxyy02
 * 03   :   call number xx function until it return false //0303xx
 * 04	:   call number xx function once
 * 			and jump to yy line if it return true or jump to zz line if it return false // 0504xxyyzz
 */
uint32_t ConditionalDoAsCommand(uint8_t* p_command,uint8_t length,uint32_t current_command_line)
{
	X_Boolean isConditional;
	uint32_t i , next_command_line;
	String_Debug(LOAD_COMMAND_DEBUG,(30,"command %2x %2x %2x; length:%d \r\n"
									,p_command[0],p_command[1],p_command[2],length));

	isConditional = X_False;
	if(p_command[1] == 0x01 || p_command[1] == 0x02 || p_command[1] == 0x03 || p_command[1] == 0x04)
	{
		switch(p_command[1])
		{
			case 0x01:
				CallFunction(p_command[2],X_Null);
				break;
			case 0x02:
				if(p_command[4] == 0x01)
				{
					for(i = 0;i<p_command[3]; i++)
					{
						CallFunction(p_command[2],X_Null);
					}
				}
				break;
			case 0x03:
				if(CallFunction(p_command[2],X_Null) == X_True)
				{
					isConditional = X_True;
					next_command_line = current_command_line - 1;// need boundary check
				}
				break;
			case 0x04:
				if(CallFunction(p_command[2],X_Null) == X_True)
				{
					isConditional = X_True;
					next_command_line = p_command[3] - 1; // need boundary check
				}
				else
				{
					isConditional = X_True;
					next_command_line = p_command[4] - 1;// need boundary check
				}
				break;
			default:
				break;


		}
		if(isConditional == X_True)
		{
			return next_command_line;
		}

	}

	return (current_command_line);// +1 is already done by read module
}

