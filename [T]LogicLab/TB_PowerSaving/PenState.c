#include "PenState.h"
#include "PowerSaving.h"
#include "pen_button.h"
#include "other_function.h"
#include "..\..\CommonSource\CharStringDebug\CharStringDebugModule.h"
#include "..\..\CommonSource\Math\bit_operation.h"

#include "EmarkConnected.h"
#include "EmarkDisconnected.h"
#include "EmarkShutDown.h"
#include "EmarkChargeWhenShutDown.h"
#include "EmarkQuiet.h"
#include "NewPenState.h"

#define PEN_PERIPHERAL_DEBUG  1
#define PEN_BASIC_STATE_DEBUG  1
STRING_DEBUG_ONCE_ENTRY_DEF(p_state_entry,100);


#define USE_POWER_LOW_SHUTDOWN 0
#define WAKE_UP_DELAY          3

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
#define WakeUpButtonPushOrNotBitPositon           7

static FactorFlag Factor;


static X_Boolean DoesUserShutDown(void)
{
	return (GetBitMethod(Factor,UserShutDownOrNotBitPosition) == 1);
}
static X_Boolean DoesBleConnected(void)
{
	return (GetBitMethod(Factor,BleConnectedOrNotBitPosition) == 1);
}
static X_Boolean DoesPenMove(void)
{
	return (GetBitMethod(Factor,PenMoveOrNotBitPosition) == 1);
}
static X_Boolean DoesPenCharge(void)
{
	return (GetBitMethod(Factor,PenChargeOrNotOrNotBitPositon) == 1);
}
static X_Boolean DoesPenNearMagnetic(void)
{
	return (GetBitMethod(Factor,PenNearMagneticOrNotBitPositon) == 1);
}
static X_Boolean DoesPenPowerLow(void)
{
	return (GetBitMethod(Factor,PenPowerLowOrNotBitPositon) == 1);
}
static X_Boolean DoesPenChargeWhenShutDown(void)
{
	return (GetBitMethod(Factor,PenChargeWhenShutDownOrNotBitPositon) == 1);
}
static X_Boolean DoesWakeUpButtonPush(void)
{
	return (GetBitMethod(Factor,WakeUpButtonPushOrNotBitPositon) == 1);
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
		case PenStateWakeUpButtonPush:
			Factor = FactorFlagSet(Factor,WakeUpButtonPushOrNotBitPositon,cs);
			break;
		default:
			break;

	}

}

static uint16_t time_stamp,delay_counter;

static X_Void PenStateInit(X_Void)
{
	PenChargeWhenShutDownStateInit();
	PenDisconnectedStateInit();
	PenConnectedStateInit();
	PenShutDownStateInit();
	PenQuietStateInit();
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
static X_Void WakeUpButtonPushedCheck(X_Void)
{
	if(UserWakeButtonStateGet() == X_True)
	{
		PenStateFactorCollector(PenStateWakeUpButtonPush,1);
	}
	else
	{
		PenStateFactorCollector(PenStateWakeUpButtonPush,0);
	}
}

static X_Boolean PenStateCollect(X_Void)
{
	UserShutDownCheck();
	DoesChargeIn();
	MotionStateCheck();
	BleStataCheck();
	MagneticStrengthCheck();
	#if (USE_POWER_LOW_SHUTDOWN == 1)
	BatteryLevelStateCollect();
	#endif

	return X_True;
}

static PenBasicState PenBasicStateGet(X_Void)
{
	if(DoesPenChargeWhenShutDown() == X_True){return PBS_ChargeWhenShutDown;}

	if(DoesPenNearMagnetic() == X_True) {return PBS_GoingToShutDown;}

	if(DoesUserShutDown() == X_True) {return PBS_GoingToShutDown;}

	#if (USE_POWER_LOW_SHUTDOWN == 1)
	if(DoesPenPowerLow() == X_True && DoesPenCharge() == X_False) {return PBS_GoingToShutDown;}
	#endif
	if(DoesBleConnected() == X_True) {return PBS_Connected;}
	else {return PBS_DisConnected;}
}

PenBasicState PenBasicStateGetUnderCertainState(PenBasicState current_state)
{
	PenBasicState pbs;

	switch(current_state)
	{
		case PBS_ChargeWhenShutDown:
			pbs = PBS_ChargeWhenShutDown;
			DoesChargeIn();
			WakeUpButtonPushedCheck();

			if(DoesPenCharge() == X_True)
			{
				if(DoesWakeUpButtonPush() == X_True){pbs = PBS_DisConnected;}
				else {pbs = PBS_ChargeWhenShutDown;}
			}
			else
			{
				if(DoesWakeUpButtonPush() == X_True){pbs = PBS_DisConnected;}
				else {pbs = PBS_GoingToShutDown;}
			}
			break;
		case PBS_Quiet:
		case PBS_DisConnected:
			BleStataCheck();
			MagneticStrengthCheck();
			DoesChargeIn();
			UserShutDownCheck();
			MotionStateCheck();
			#if (USE_POWER_LOW_SHUTDOWN == 1)
			BatteryLevelStateCollect();
			#endif
			if(DoesUserShutDown() == X_True || DoesPenNearMagnetic() == X_True)
			{
				pbs = PBS_GoingToShutDown;
			}
			#if (USE_POWER_LOW_SHUTDOWN == 1)
			else if(DoesPenPowerLow() == X_True && DoesPenCharge() == X_False)
			{
				pbs = PBS_GoingToShutDown;
			}
			#endif
			else if(DoesBleConnected() == X_True)
			{
				pbs = PBS_Connected;
			}
			else if(DoesPenMove() == X_False)
			{
				pbs = PBS_Quiet;
			}
			else
			{
				pbs = PBS_DisConnected;
			}
			break;
		case PBS_Connected:
			BleStataCheck();
			MagneticStrengthCheck();
			DoesChargeIn();
			UserShutDownCheck();
			#if (USE_POWER_LOW_SHUTDOWN == 1)
			BatteryLevelStateCollect();
			#endif

			if(DoesUserShutDown() == X_True || DoesPenNearMagnetic() == X_True)
			{
				pbs = PBS_GoingToShutDown;
			}
			#if (USE_POWER_LOW_SHUTDOWN == 1)
			else if(DoesPenPowerLow() == X_True && DoesPenCharge() == X_False)
			{
				pbs = PBS_GoingToShutDown;
			}
			#endif
			else if(DoesBleConnected() == X_False)
			{
				pbs = PBS_DisConnected;
			}
			else
			{
				pbs = PBS_Connected;
			}
			break;
		case PBS_GoingToShutDown:
			pbs = PBS_GoingToShutDown;
			break;
		default:
			break;
	}

	return pbs;
}

static PenBasicState CurrentState;

X_Boolean SetCurrentBasicState(PenBasicState state)
{
	X_Boolean isOK;

	isOK = X_True;
	if(state == CurrentState)
	{
		return isOK;
	}
	switch(state)
	{
		case PBS_ChargeWhenShutDown:
			PenChargeWhenShutDownStateInit();
			CurrentState = PBS_ChargeWhenShutDown;
			break;
		case PBS_DisConnected:
			PenDisconnectedStateInit();
			CurrentState = PBS_DisConnected;
			break;
		case PBS_Connected:
			PenConnectedStateInit();
			CurrentState = PBS_Connected;
			break;
		case PBS_GoingToShutDown:
		    PenShutDownStateInit();
			CurrentState = PBS_GoingToShutDown;
			break;
		case PBS_Quiet:
			PenQuietStateInit();
			CurrentState = PBS_Quiet;
			break;
		default:
			isOK = X_False;
			break;
	}
	return isOK;
}

static PenBasicState JustWakeUpAction(X_Void)
{
//	String_Debug_Once(PEN_BASIC_STATE_DEBUG,p_state_entry,PBS_JustWakeUp,(30,"--JustWakeUp\r\n"));

	if(delay_counter < WAKE_UP_DELAY)
	{
		delay_counter ++;
		return PBS_JustWakeUp;
	}
	else
	{
		delay_counter = 0;
		return PBS_GetGlobalState;
	}


}
static PenBasicState GetGlobalStateAction(X_Void)
{
	PenBasicState state;
//	String_Debug_Once(PEN_BASIC_STATE_DEBUG,p_state_entry,PBS_GetGlobalState,(30,"--GetGlobalState\r\n"));
	PenStateCollect();
	state = PenBasicStateGet();
	SetCurrentBasicState(state);// for init
	return state;


}
static PenBasicState ChargeWhenShutDownAction(X_Void)
{
	String_Debug_Once(PEN_BASIC_STATE_DEBUG,p_state_entry,PBS_ChargeWhenShutDown,(30,"--ChargeWhenShutDown\r\n"));
	PenChargeWhenShutDownStateHandle();
	return CurrentState;
}
static PenBasicState DisConnectedAction(X_Void)
{
	String_Debug_Once(PEN_BASIC_STATE_DEBUG,p_state_entry,PBS_DisConnected,(30,"--DisConnected\r\n"));
	PenDisconnectedStateHandle();
	return CurrentState;
}
static PenBasicState ConnectedAction(X_Void)
{
	String_Debug_Once(PEN_BASIC_STATE_DEBUG,p_state_entry,PBS_Connected,(30,"--Connected\r\n"));
	PenConnectedStateHandle();
	return CurrentState;
}
static PenBasicState GoingToShutDownAction(X_Void)
{
	String_Debug_Once(PEN_BASIC_STATE_DEBUG,p_state_entry,PBS_GoingToShutDown,(30,"--GoingToShutDown\r\n"));
	PenShutDownStateHandle();
	return CurrentState;
}
static PenBasicState QuietAction(X_Void)
{
	String_Debug_Once(PEN_BASIC_STATE_DEBUG,p_state_entry,PBS_Quiet,(30,"--Quiet\r\n"));
	PenQuietStateHandle();
	return CurrentState;
}

static struct _StateHandle{
	PenBasicState state;
	PenBasicState (*Action)(X_Void);
}
const StateHandle[]={
	{PBS_JustWakeUp,JustWakeUpAction},
	{PBS_GetGlobalState,GetGlobalStateAction},
	{PBS_ChargeWhenShutDown,ChargeWhenShutDownAction},
	{PBS_DisConnected,DisConnectedAction},
	{PBS_Connected,ConnectedAction},
	{PBS_GoingToShutDown,GoingToShutDownAction},
	{PBS_Quiet,QuietAction},
};

static X_Void PenBasicStateHandle(X_Void)
{
	uint8_t i, state_number;

	state_number = (sizeof(StateHandle) / sizeof(StateHandle[0]));

	for(i=0;i< state_number ;i++)
	{
		if(StateHandle[i].state == CurrentState)
		{
			if(StateHandle[i].Action != X_Null)
			{
				CurrentState = StateHandle[i].Action();
				break;
			}
		}
	}

	if(i >= state_number)
	{
		String_Debug(PEN_PERIPHERAL_DEBUG,(30,"unknow state\r\n"));
		CurrentState = PBS_JustWakeUp;
	}

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
	ExternalPowerHandle();//and PowerShutDownMonitor();
	CapPowerHandle();
	RGB_DisplayHandle();
//	ChargeIncheckHandle();

}

static X_Void BleAndPeripheralAction(X_Void)
{
	time_stamp ++;
//	PenBasicStateHandle();
	AllStateAndEventAction();
	DoTheAction();
	AlwaysRunningAction();
}

X_Void AllPeripheralInit(X_Void)
{
	CurrentState = PBS_JustWakeUp;
	time_stamp = 0;
	delay_counter = 0;
	PenStateInit();
}

void onTick(void)
{
	if(DoesUserAppLocked() == X_False)
	{
		BleAndPeripheralAction();
	}

}
/*
command for pen state:

:command:07550100010100
:command:07550200010100
:command:07550d00010100
:command:07550b00010100
:command:07550400010114
:command:07550500010100
:command:07550700010100
:command:07550600010100
:command:07550800010164
:command:07550c0001010a
:command:07550900010108
:command:075513010e0d00
:command:07550e01050d00
:command:07550d00010100
:command:07550a00010100
:command:0755040001010a
:command:07551000010108
:command:07551100010100
:command:07551200010100
:command:07550301151405
:command:07550e01041500
:end
*/
