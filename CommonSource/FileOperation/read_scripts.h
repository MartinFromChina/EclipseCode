#ifndef __READ_SCRIPTS_H
#define __READ_SCRIPTS_H


#include "..\KeilMDK.h"
#include "read_scripts_debug_switch.h"

//#include "command.h"
//
//X_Boolean GetCommandFromOneTxtLine(CommandParam *p_commparam);
void ReadScriptsInit(FILE* (*open_file)(void));

#endif
