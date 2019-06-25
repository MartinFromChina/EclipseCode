#include "other_function.h"
#include "PowerSaving.h"
#include "PenState.h"

void MCU_ResetHandle(void)
{
//	if(isMCU_Reset == X_True)
//	{
//		//sth that must be done before reset
//		if(MCU_ResetHoldCounter > 0)
//		{
//			MCU_ResetHoldCounter -- ;
//			return ;
//		}
//		//
//		if(isMCU_ResetReady == X_True)
//		{
//			MCU_Reset();
//		}
//	}
}
void CalibrateMotionSensorHandle(void)
{

}
void CollectADCValueHandle(void)
{

}
void EmarkPenCommandHandle(void)
{

}
void ExternalPowerHandle(void)
{

}
void CapPowerHandle(void){

}
void RGB_DisplayHandle(void){}
void MotionStateCheck(void)
{
	if(PenMoveStateGet() == X_True)
	{
		PenStateFactorCollector(PenStatePenMove,1);
	}
	else
	{
		PenStateFactorCollector(PenStatePenMove,0);
	}
}
void SetAdcRefreshEnable(X_Boolean isenable)
{

}
void AdcGetSum(void){}
void BatteryStrengthMonitor(void){}
void BatteryLevelStateCollect(void)
{
	if(PowerStateGet() == X_True)
	{
		PenStateFactorCollector(PenStatePenPowerLow,1);
	}
	else
	{
		PenStateFactorCollector(PenStatePenPowerLow,0);
	}
}
void AllEmkCharacteristicAction(void){}
void CharacteristicBatteryHandle(void){}
void DoesChargeIn(void)
{
	if(BatteryIsChargerConnected() == X_True)
	{
		PenStateFactorCollector(PenStatePenCharge,1);
	}
	else
	{
		PenStateFactorCollector(PenStatePenCharge,0);
	}
}
X_Boolean BleSleepAndPowerSavingHandle(void)
{
	return X_False;
}
