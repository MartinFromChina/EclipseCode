#include "PenState.h"
#include "PowerSaving.h"
#include "pen_button.h"
#include "other_function.h"
#include "..\..\CommonSource\CharStringDebug\CharStringDebugModule.h"
#include "..\..\CommonSource\Math\bit_operation.h"

#define PEN_PERIPHERAL_DEBUG  1

#define  FactorFlag   uint8_t
#define  MaxBitNumber   8
#define  FactorMask							((FactorFlag)(1u << 0))

#define  ClearBitMethod   byte_clearbit
#define  GetBitMethod     byte_getbit
#define  TurnBitMethod    byte_bitoverturn
#define  FactorFlagSet    ByteFactorSet

#define UserShutDownOrNotBitPosition      0
#define BleConnectedOrNotBitPosition  	  1
#define PenMoveOrNotBitPosition   		  2
#define PenChargeOrNotOrNotBitPositon	  3
#define PenNearMagneticOrNotBitPositon    4
#define PenPowerLowOrNotBitPositon  	  5
#define PenChargeWhenShutDownOrNotBitPositon  	  6

#define ToggleBitPosition                 7

static FactorFlag Factor;


static X_Boolean IsUserShutDown(void)
{
	return (GetBitMethod(Factor,UserShutDownOrNotBitPosition) == 1);
}
static X_Boolean IsBleConnected(void)
{
	return (GetBitMethod(Factor,BleConnectedOrNotBitPosition) == 1);
}
static X_Boolean IsPenMove(void)
{
	return (GetBitMethod(Factor,PenMoveOrNotBitPosition) == 1);
}
static X_Boolean IsPenCharge(void)
{
	return (GetBitMethod(Factor,PenChargeOrNotOrNotBitPositon) == 1);
}
static X_Boolean IsPenNearMagnetic(void)
{
	return (GetBitMethod(Factor,PenNearMagneticOrNotBitPositon) == 1);
}
static X_Boolean IsPenPowerLow(void)
{
	return (GetBitMethod(Factor,PenPowerLowOrNotBitPositon) == 1);
}

void PenStateFactorCollector(FactorAboutPenState factor_mul,uint32_t factor)
{
	X_Boolean cs;
	cs = (factor == 0)? Clear : Set;
	switch(factor_mul)
	{
		case PenStateUserShutDown:
			Factor = FactorFlagSet(Factor,UserShutDownOrNotBitPosition,cs);
			break;
		case PenStateBleConnected:
			Factor = FactorFlagSet(Factor,BleConnectedOrNotBitPosition,cs);
			break;
		case PenStatePenMove:
			Factor = FactorFlagSet(Factor,PenMoveOrNotBitPosition,cs);
			break;
		case PenStatePenCharge:
			Factor = FactorFlagSet(Factor,PenChargeOrNotOrNotBitPositon,cs);
			break;
		case PenStatePenNearMagnetic:
			Factor = FactorFlagSet(Factor,PenNearMagneticOrNotBitPositon,cs);
			break;
		case PenStatePenPowerLow:
			Factor = FactorFlagSet(Factor,PenPowerLowOrNotBitPositon,cs);
			break;
		case PenStatePenChargeWhenShutDown:
			Factor = FactorFlagSet(Factor,PenChargeWhenShutDownOrNotBitPositon,cs);
			break;
		default:
			break;

	}

}


static PenState ePS = DisConnected;
static uint16_t time_stamp;

static X_Void PenStateInit(X_Void)
{
	/**********
	//mcu reset just happen and charge in,it must be charge in wake up the MCU
	*********************/
	if(BatteryIsChargerConnected() == X_True)
	{
		PenStateFactorCollector(PenStatePenChargeWhenShutDown,1);
	}
	else
	{
		PenStateFactorCollector(PenStatePenChargeWhenShutDown,0);
	}
}
static X_Void BleStataCheck(X_Void)
{
	if(BleStateGet() == X_True)
	{
		PenStateFactorCollector(PenStateBleConnected,1);
	}
	else
	{
		PenStateFactorCollector(PenStateBleConnected,0);
	}
}
static X_Void MagneticStrengthCheck(X_Void)
{
	if(MagneticStateGet() == X_True)
	{
		PenStateFactorCollector(PenStatePenNearMagnetic,1);
	}
	else
	{
		PenStateFactorCollector(PenStatePenNearMagnetic,0);
	}
}
static X_Void UserShutDownCheck(X_Void)
{
	if(UserShutDownStateGet() == X_True)
	{
		PenStateFactorCollector(PenStateUserShutDown,1);
	}
	else
	{
		PenStateFactorCollector(PenStateUserShutDown,0);
	}
}
static X_Boolean PenStateCollect(X_Void)
{
	UserShutDownCheck();
	DoesChargeIn();
	MotionStateCheck();
	BleStataCheck();
	MagneticStrengthCheck();
	return X_True;
}
static uint8_t FactorBackup = 0;
static X_Void PenStateController(X_Boolean isChanged)
{
	if(FactorBackup != Factor)
	{
		String_Debug(PEN_PERIPHERAL_DEBUG,(30,"Factor : %2x\r\n",Factor));
	}
	FactorBackup = Factor;

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


