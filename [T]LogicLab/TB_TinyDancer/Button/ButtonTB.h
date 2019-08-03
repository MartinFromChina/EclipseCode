#ifndef __BUTTONTB_H
#define __BUTTONTB_H

#include "..\..\UserDebug.h"
#define MAX_BUTTON_VALUE  200

X_Boolean CallFunction(uint8_t func_num,X_Void * p_param);
uint32_t  GetCurrentButtonValue(X_Void);

#endif
