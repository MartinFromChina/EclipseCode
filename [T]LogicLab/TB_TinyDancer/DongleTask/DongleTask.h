#ifndef __DONGLETASK_H
#define __DONGLETASK_H

#include "..\..\..\CommonSource\KeilMDK.h"

#define DONGLE_TASK_INTERVAL_IN_MS       7

X_Void timer_init(X_Void);
X_Void StartTaskTimer(X_Void);
X_Void DongleTaskInit(X_Void);

#endif
