#ifndef __HEXCOMMAND_H
#define __HEXCOMMAND_H

#include "..\CommonSource\FileOperation\read_scripts.h"

#define LOAD_COMMAND_DEBUG 1

X_Boolean DoAsCommand(uint8_t* p_command,uint8_t length);
uint32_t ConditionalDoAsCommand(uint8_t* p_command,uint8_t length,uint32_t current_command_line);

#endif
