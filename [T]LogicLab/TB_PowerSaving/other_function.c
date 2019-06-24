#include "other_function.h"
#include "PowerSaving.h"

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
void MotionStateCheck(void){}
void SetAdcRefreshEnable(X_Boolean isenable)
{

}
void AdcGetSum(void){}
void BatteryStrengthMonitor(void){}
void BatteryLevelStateCollect(void){}
void AllEmkCharacteristicAction(void){}
void CharacteristicBatteryHandle(void){}
X_Boolean DoesChargeIn(void)
{
	return BatteryIsChargerConnected();
}
X_Boolean BleSleepAndPowerSavingHandle(void)
{

}
