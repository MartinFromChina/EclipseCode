#ifndef __HEXCOMMAND_H
#define __HEXCOMMAND_H

#include "..\CommonSource\FileOperation\read_scripts.h"

#define ScriptFunctionParamBufLength    10

typedef struct
{
	X_Boolean isParamValid;
	uint8_t   recent_pass_function_number;
	uint8_t   param_from_script;
	uint8_t   buf_8[ScriptFunctionParamBufLength];
	uint16_t  buf_16[ScriptFunctionParamBufLength];
	uint32_t  buf_32[ScriptFunctionParamBufLength];
}ScriptFunctionParam;

X_Boolean DoAsCommand(uint8_t* p_command,uint8_t length);
uint32_t ConditionalDoAsCommand(uint8_t* p_command,uint8_t length,uint32_t current_command_line);

#endif
