#ifndef __PENSTATE_H
#define __PENSTATE_H

#include "..\..\CommonSource\KeilMDK.h"

typedef enum
{
	ChargeWhenShutDown,
	DisConnected,
	Connected,
	GoingToShutDown,
}PenState;

X_Void AllPeripheralInit(X_Void);
void onTick(void);

#endif
