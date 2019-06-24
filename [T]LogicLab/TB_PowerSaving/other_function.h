#ifndef __OTHER_FUNCTION_H
#define __OTHER_FUNCTION_H

#include "..\..\CommonSource\KeilMDK.h"

void MCU_ResetHandle(void);
void CalibrateMotionSensorHandle(void);
void CollectADCValueHandle(void);
void EmarkPenCommandHandle(void);
void ExternalPowerHandle(void);
void CapPowerHandle(void);
void RGB_DisplayHandle(void);
void MotionStateCheck(void);
void SetAdcRefreshEnable(X_Boolean isenable);
void AdcGetSum(void);
void BatteryStrengthMonitor(void);
void BatteryLevelStateCollect(void);
void AllEmkCharacteristicAction(void);
void CharacteristicBatteryHandle(void);
void DoesChargeIn(void);
X_Boolean BleSleepAndPowerSavingHandle(void);

#endif
