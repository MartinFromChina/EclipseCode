#ifndef __DONGLETASKTB_H
#define __DONGLETASKTB_H

#include "..\..\..\..\CommonSource\KeilMDK.h"
#include <stdio.h>

FILE * DongleTaskOpenFile(void);
X_Boolean CallFunction(uint8_t func_num,X_Void * p_param);

#endif
