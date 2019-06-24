#ifndef __POWERSAVING_H
#define __POWERSAVING_H

#include "..\..\CommonSource\KeilMDK.h"

X_Boolean CallFunction(uint8_t func_num,X_Void * p_param);

X_Boolean BatteryIsChargerConnected(X_Void);
X_Boolean PenMoveStateGet(X_Void);
X_Boolean BleStateGet(X_Void);
X_Boolean MagneticStateGet(X_Void);
X_Boolean PowerStateGet(X_Void);
X_Boolean UserShutDownStateGet(X_Void);
X_Boolean DoesUserAppLocked(X_Void);
#endif
