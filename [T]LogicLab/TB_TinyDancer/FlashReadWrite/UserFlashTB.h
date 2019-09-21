#ifndef __USERFLASHTB_H
#define __USERFLASHTB_H

#include "..\..\UserDebug.h"

#define MAX_SECTOR_NUMBER    10

X_Boolean CallFunction(uint8_t func_num,X_Void * p_param);
X_Boolean  GetSectorNumber(uint8_t *p_sector);
X_Boolean DoesFlashReady(X_Void);

#endif
