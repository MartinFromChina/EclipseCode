#ifndef __PENSTATE_H
#define __PENSTATE_H

#include "..\..\CommonSource\KeilMDK.h"

typedef enum
{
	PenStateUserShutDown,
	PenStateBleConnected,
	PenStatePenMove,
	PenStatePenCharge,
	PenStatePenNearMagnetic,
	PenStatePenPowerLow,
	PenStatePenChargeWhenShutDown,
}FactorAboutPenState;

typedef enum
{
	ChargeWhenShutDown,
	DisConnected,
	Connected,
	GoingToShutDown,
}PenState;

X_Void AllPeripheralInit(X_Void);
void onTick(void);
void PenStateFactorCollector(FactorAboutPenState factor_mul,uint32_t factor);

#endif
