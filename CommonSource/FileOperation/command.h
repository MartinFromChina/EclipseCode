#ifndef __COMMAND_H
#define __COMMAND_H

#include "..\KeilMDK.h"
#include "read_scripts.h"

#define MaxCommandBufNumber  	10
#define MaxCommandLength    	20
#define MaxCharToHexTableSize   55
#define CommandStartAddr      	 9// ":command:" have 9 char
#define CommandLengthAddr  		 0

void CommandReceivedInit(void);
X_Boolean CommandAnalysis(char * command_string,ScriptCommandParam *p_commparam);
X_Boolean LoadCommand(uint8_t **p_command_value,uint8_t *length);
#endif
