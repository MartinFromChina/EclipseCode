#include "HexCommand.h"
#include "..\CommonSource\CharStringDebug\CharStringDebugModule.h"

X_Boolean DoAsCommand(uint8_t* p_command,uint8_t length)
{
	String_Debug(LOAD_COMMAND_DEBUG,(30,"command %2x %2x %2x; length:%d \r\n"
									,p_command[0],p_command[1],p_command[2],length));

	return X_True;
}

uint32_t ConditionalDoAsCommand(uint8_t* p_command,uint8_t length,uint32_t current_command_line)
{
	String_Debug(LOAD_COMMAND_DEBUG,(30,"command %2x %2x %2x; length:%d \r\n"
									,p_command[0],p_command[1],p_command[2],length));

	if(p_command[0] == 0x03 && p_command[1] == 0x55)
	{
		return p_command[2];
	}

	return (current_command_line);// +1 is already done by read module
}

