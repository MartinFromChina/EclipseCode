#ifndef __DONGLETASKTB_H
#define __DONGLETASKTB_H

#include "..\..\..\..\CommonSource\KeilMDK.h"
#include <stdio.h>

typedef struct {
	uint8_t number;
	uint8_t command;
	char *  p_char;
}_sCommandMap;

uint32_t GetCurrentRandomNumber(X_Void);
FILE * DongleTaskOpenFile(void);
X_Boolean CallFunction(uint8_t func_num,X_Void * p_param);

#endif
