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
	PenStateWakeUpButtonPush,
}FactorAboutPenState;

typedef enum
{
	PBS_JustWakeUp,
	PBS_GetGlobalState,
	PBS_ChargeWhenShutDown,
	PBS_DisConnected,
	PBS_Connected,
	PBS_GoingToShutDown,
	PBS_Quiet,// sleep caused by pen in static state for a long time
}PenBasicState;

X_Void AllPeripheralInit(X_Void);
void onTick(void);
void PenStateFactorCollector(FactorAboutPenState factor_mul,uint32_t factor);
PenBasicState PenBasicStateGetUnderCertainState(PenBasicState current_state);
X_Boolean SetCurrentBasicState(PenBasicState state);

#endif
