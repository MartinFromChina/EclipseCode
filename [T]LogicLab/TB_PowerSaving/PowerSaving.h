#ifndef __POWERSAVING_H
#define __POWERSAVING_H

#include "..\..\CommonSource\KeilMDK.h"

X_Boolean CallFunction(uint8_t func_num,X_Void * p_param);

X_Boolean BatteryIsChargerConnected(X_Void);
#endif
