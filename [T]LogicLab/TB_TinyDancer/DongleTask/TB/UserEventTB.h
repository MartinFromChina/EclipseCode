#ifndef __USEREVENTTB_H
#define __USEREVENTTB_H

#include "..\..\..\..\CommonSource\KeilMDK.h"

typedef enum
{
	SuddenlyPowerOff = 0,
	ResetByWatchDog,
	ResetByUser,
	HardFault,
}EventUser;

X_Boolean DoesResetHappen(X_Void);
X_Void ResetUserEventState(X_Void);

#endif
