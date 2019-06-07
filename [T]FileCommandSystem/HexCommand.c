#include "HexCommand.h"
#include "..\CommonSource\CharStringDebug\CharStringDebugModule.h"

X_Boolean DoAsCommand(uint8_t* p_command,uint8_t length)
{
	String_Debug(LOAD_COMMAND_DEBUG,(30,"command %2x %2x %2x; length:%d \r\n"
									,p_command[0],p_command[1],p_command[2],length));

	return X_True;
}
