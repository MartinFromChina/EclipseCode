#ifndef __READ_SCRIPTS_H
#define __READ_SCRIPTS_H


#include "..\KeilMDK.h"
#include "read_scripts_debug_switch.h"

typedef enum
{
	UnknowCommand,
	KnowCommand,
	Wait,
	NoCommandAnyMore,
}ScriptCommandType;

typedef struct
{
	ScriptCommandType  command;
	uint32_t wait_time;
}ScriptCommandParam;

void ReadScriptsInit(FILE* (*open_file)(void));
void ScriptCommandHandle(X_Boolean(*doAsCommand)(uint8_t* p_command,uint8_t length));

#endif
