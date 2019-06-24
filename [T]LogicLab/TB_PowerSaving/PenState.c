#include "PenState.h"
#include "PowerSaving.h"
#include "pen_button.h"
#include "other_function.h"
#include "..\..\CommonSource\CharStringDebug\CharStringDebugModule.h"



#define PEN_PERIPHERAL_DEBUG  1

static PenState ePS = DisConnected;
static uint16_t time_stamp;

static X_Void PenStateInit(X_Void)
{
	/**********
	//mcu reset just happen and charge in,it must be charge in wake up the MCU
	*********************/
	if(BatteryIsChargerConnected() == X_True)
	{
		ePS = ChargeWhenShutDown;
	}
	else
	{
		ePS = DisConnected;
		String_Debug(PEN_PERIPHERAL_DEBUG,(40,"tell RGB Module MCU just wake up\r\n"));
	}
}

static X_Boolean PenStateCollect(X_Void)
{
	DoesChargeIn();
	MotionStateCheck();
	return X_True;
}
static X_Void PenStateController(X_Boolean isChanged)
{

}
static X_Void DoTheAction(X_Void)
{
//	SetAdcRefreshEnable(X_True);
//	AdcGetSum();
//	BatteryStrengthMonitor();
//	BatteryLevelStateCollect();
//	AllEmkCharacteristicAction();
//	CharacteristicBatteryHandle();
}
static X_Void AlwaysRunningAction(X_Void)
{
	X_Boolean isShutDownForSure;
	isShutDownForSure = BleSleepAndPowerSavingHandle();
	ButtonActionHandle();
	MCU_ResetHandle();
	CollectADCValueHandle();
	EmarkPenCommandHandle();
	ExternalPowerHandle();
	CapPowerHandle();
	RGB_DisplayHandle();

}

static X_Void BleAndPeripheralAction(X_Void)
{
	time_stamp ++;
	X_Boolean isStateChange;

	isStateChange = PenStateCollect();
	PenStateController(isStateChange);
	DoTheAction();
	AlwaysRunningAction();
}

X_Void AllPeripheralInit(X_Void)
{
	time_stamp = 0;
	PenStateInit();
}

void onTick(void)
{
	BleAndPeripheralAction();
}


