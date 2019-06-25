#include "PowerSaving.h"
#include "PenState.h"
#include "EmarkShutDown.h"
#include "EmarkQuiet.h"
#include "..\ScriptsCommandModule\HexCommand.h"
#include "..\..\CommonSource\Math\random_number.h"
#include "..\..\CommonSource\CharStringDebug\CharStringDebugModule.h"

STRING_DEBUG_ONCE_ENTRY_DEF(p_move_entry,0);
STRING_DEBUG_ONCE_ENTRY_DEF(p_ble_entry,0);
STRING_DEBUG_ONCE_ENTRY_DEF(p_magnetic_entry,0);
STRING_DEBUG_ONCE_ENTRY_DEF(p_power_entry,0);

#define SCRIPT_FUNCTION_DEBUG		1

#define PARAM_FROM_SCRIPT_ADDRESS   		0
#define LOOP_COUNTER_ADDRESS        		1
#define CURRENT_RANDOM_NUM_ADDRESS   		2
#define CHARGE_FUNCTION_PARAM_ADDRESS   	3
#define MAGNETIC_FUNCTION_PARAM_ADDRESS   	4
#define LOOP_COUNTER_THRESHOLD_ADDRESS      5
#define BLE_FUNCTION_PARAM_ADDRESS       	6
#define POWER_FUNCTION_PARAM_ADDRESS   		7

#define SECOND_CONDITION_JUMP_ADDRESS  ScriptFunctionParamBufLength

static X_Boolean BatteryChargeStateToggle(X_Void);
static X_Void BatteryChargeInit(X_Void);
static X_Void PenMoveStateInit(X_Void);
static X_Void PenMoveStateSet(X_Boolean isMove);
static X_Void BleStateInit(X_Void);
static X_Void BleStateSet(X_Boolean isOK);
static X_Void MagneticStateInit(X_Void);
static X_Void MagneticStateSet(X_Boolean isOK);
static X_Void PowerStateInit(X_Void);
static X_Void PowerStateSet(X_Boolean isOK);
static X_Void UserShutDownStateInit(X_Void);
static X_Void UserShutDownStateSet(X_Boolean isOK);
static X_Void UserWakeButtonStateInit(X_Void);
static X_Void UserWakeButtonStateSet(X_Boolean isOK);

static X_Boolean isUserAppLocked;

static X_Boolean UserAppLock(X_Void * p_param)
{
	isUserAppLocked = X_True;
	return X_True;
}
static X_Boolean UserAppUnlock(X_Void * p_param)
{
	isUserAppLocked = X_False;
	return X_True;
}
X_Boolean DoesUserAppLocked(X_Void)
{
	return isUserAppLocked;
}

static X_Boolean ScriptsFunctionInitial(X_Void * p_param)
{
	uint8_t i;
//	String_Debug(SCRIPT_FUNCTION_DEBUG,(30,"1 :initial is called\r\n"));

	ScriptFunctionParam * p_SFP;
	p_SFP = (ScriptFunctionParam *)p_param;

	p_SFP->isParamValid = X_False;
	p_SFP->recent_pass_function_number = 0;
	p_SFP->param_from_script = 0;

	for(i=0;i<ScriptFunctionParamBufLength;i++)
	{
		p_SFP->buf_8[i]		= 0;
		p_SFP->buf_16[i]	= 0;
		p_SFP->buf_32[i]	= 0;
	}

	return X_True;
}
static X_Boolean ForNow(X_Void * p_param)
{
	ScriptFunctionParam * p_SFP;
	p_SFP = (ScriptFunctionParam *)p_param;

	p_SFP ->isParamValid = X_True;
	p_SFP ->buf_8[PARAM_FROM_SCRIPT_ADDRESS] = p_SFP->param_from_script;

//	String_Debug(SCRIPT_FUNCTION_DEBUG,(30,"2 : ForNow is called \r\n"));

	BatteryChargeInit();
	PenMoveStateInit();
	BleStateInit();
	MagneticStateInit();
	PowerStateInit();
	UserShutDownStateInit();
	UserWakeButtonStateInit();

	return X_True;
}
static X_Boolean LoopSpecificTimes(X_Void * p_param)
{
	ScriptFunctionParam * p_SFP;
	p_SFP = (ScriptFunctionParam *)p_param;

//	String_Debug(SCRIPT_FUNCTION_DEBUG,(30,"3 : Loop is called : %d\r\n",p_SFP ->buf_8[0]));
	p_SFP ->isParamValid = X_True;
	if(p_SFP ->buf_8[LOOP_COUNTER_ADDRESS]+1 >= p_SFP ->param_from_script)
	{
		p_SFP ->buf_8[LOOP_COUNTER_ADDRESS] = 0;
		return X_True;
	}
	else
	{
		p_SFP ->buf_8[LOOP_COUNTER_ADDRESS] ++;
		return X_False;
	}
}
static X_Boolean RandomNumberGenerator(X_Void * p_param)
{
	uint32_t random_num;

	ScriptFunctionParam * p_SFP;
	p_SFP = (ScriptFunctionParam *)p_param;

	random_num = GetRandomNumber(1,p_SFP ->param_from_script);
//	String_Debug(SCRIPT_FUNCTION_DEBUG,(30,"4 : random_num %d\r\n",random_num));

	p_SFP ->isParamValid = X_True;
	p_SFP ->buf_8[CURRENT_RANDOM_NUM_ADDRESS] = (uint8_t)random_num;

	return X_True;
}

static X_Boolean UserShutDownGenerator(X_Void * p_param)
{
	uint8_t random_num;
	ScriptFunctionParam * p_SFP;

	p_SFP = (ScriptFunctionParam *)p_param;
	random_num = p_SFP ->buf_8[CURRENT_RANDOM_NUM_ADDRESS];

	// get app state

	if(random_num == 20)
	{
		String_Debug(SCRIPT_FUNCTION_DEBUG,(30,"user shut down the pen\r"));
		p_SFP ->buf_8[SECOND_CONDITION_JUMP_ADDRESS] = 1;
		UserShutDownStateSet(X_True);
	}
	else
	{
		p_SFP ->buf_8[SECOND_CONDITION_JUMP_ADDRESS] = 0;
		UserShutDownStateSet(X_False);
	}

	if(random_num %2 == 0 && random_num >10)
	{
		String_Debug(SCRIPT_FUNCTION_DEBUG,(30,"wake up button pushed \r"));
		UserWakeButtonStateSet(X_True);
	}
	else
	{
		UserWakeButtonStateSet(X_False);
	}

	return X_True;
}
static X_Boolean BleStateGenerator(X_Void * p_param)
{
	uint8_t random_num;
	ScriptFunctionParam * p_SFP;

	p_SFP = (ScriptFunctionParam *)p_param;
	random_num = p_SFP ->buf_8[CURRENT_RANDOM_NUM_ADDRESS];

	// get app state

	p_SFP ->buf_8[BLE_FUNCTION_PARAM_ADDRESS] = p_SFP ->buf_8[BLE_FUNCTION_PARAM_ADDRESS] + random_num;
	if(p_SFP ->buf_8[BLE_FUNCTION_PARAM_ADDRESS] > 40 && p_SFP ->buf_8[BLE_FUNCTION_PARAM_ADDRESS] < 80)
	{
		String_Debug_Once(SCRIPT_FUNCTION_DEBUG,p_ble_entry,1,(30,"ble connected \r"));
		BleStateSet(X_True);
	}
	else if(p_SFP ->buf_8[BLE_FUNCTION_PARAM_ADDRESS] <= 80)
	{

		String_Debug_Once(SCRIPT_FUNCTION_DEBUG,p_ble_entry,2,(30,"ble disconnected \r"));
		BleStateSet(X_False);
	}
	else
	{
		p_SFP ->buf_8[BLE_FUNCTION_PARAM_ADDRESS] = 0;
	}

	return X_True;
}
static X_Boolean PenMoveStateGenerator(X_Void * p_param)
{
	uint8_t random_num;
	ScriptFunctionParam * p_SFP;

	p_SFP = (ScriptFunctionParam *)p_param;
	random_num = p_SFP ->buf_8[CURRENT_RANDOM_NUM_ADDRESS];

	// get app state

	if(random_num % 4 == 0 || random_num % 6 == 0)
	{
		String_Debug_Once(SCRIPT_FUNCTION_DEBUG,p_move_entry,1,(30,"pen silence\r"));
		PenMoveStateSet(X_False);
	}
	else if(random_num % 3 == 0 || random_num % 5 == 0)
	{
		String_Debug_Once(SCRIPT_FUNCTION_DEBUG,p_move_entry,2,(30,"pen move\r"));
		PenMoveStateSet(X_True);
	}
	return X_True;
}
static X_Boolean PenChargeStateGenerator(X_Void * p_param)
{
	uint8_t random_num;
	X_Boolean isOK;
	ScriptFunctionParam * p_SFP;

	p_SFP = (ScriptFunctionParam *)p_param;
	random_num = p_SFP ->buf_8[CURRENT_RANDOM_NUM_ADDRESS];

	// get app state

	if(p_SFP ->buf_8[CHARGE_FUNCTION_PARAM_ADDRESS] >= p_SFP ->param_from_script )
	{
		p_SFP ->buf_8[CHARGE_FUNCTION_PARAM_ADDRESS] = 0;
		isOK = BatteryChargeStateToggle();
		String_Debug(SCRIPT_FUNCTION_DEBUG,(40,"pen charge state change %d\r\n",isOK));
	}
	else
	{
		p_SFP ->buf_8[CHARGE_FUNCTION_PARAM_ADDRESS] = p_SFP ->buf_8[CHARGE_FUNCTION_PARAM_ADDRESS] + random_num;
	}
	return X_True;
}
static X_Boolean QuickPenChargeStateGenerator(X_Void * p_param)
{
	uint8_t random_num;
	X_Boolean isOK;
	ScriptFunctionParam * p_SFP;

	p_SFP = (ScriptFunctionParam *)p_param;
	random_num = p_SFP ->buf_8[CURRENT_RANDOM_NUM_ADDRESS];

	// get app state

	if(random_num >= p_SFP ->param_from_script )
	{
		isOK = BatteryChargeStateToggle();
		String_Debug(SCRIPT_FUNCTION_DEBUG,(40,"pen charge state change %d --\r",isOK));
	}
	return X_True;
}
static X_Boolean PenDistanceFromMagneticGenerator(X_Void * p_param)
{
	uint8_t random_num;
	ScriptFunctionParam * p_SFP;

	p_SFP = (ScriptFunctionParam *)p_param;
	random_num = p_SFP ->buf_8[CURRENT_RANDOM_NUM_ADDRESS];

	// get app state

	p_SFP ->buf_8[MAGNETIC_FUNCTION_PARAM_ADDRESS] = p_SFP ->buf_8[MAGNETIC_FUNCTION_PARAM_ADDRESS] + random_num;
	if(p_SFP ->buf_8[MAGNETIC_FUNCTION_PARAM_ADDRESS] >= 200 && p_SFP ->buf_8[MAGNETIC_FUNCTION_PARAM_ADDRESS] <= 210 )
	{
		String_Debug_Once(SCRIPT_FUNCTION_DEBUG,p_magnetic_entry,1,(30,"pen near magnetic\r"));
		p_SFP ->buf_8[MAGNETIC_FUNCTION_PARAM_ADDRESS] = 0;
		MagneticStateSet(X_True);
	}
	else if (p_SFP ->buf_8[MAGNETIC_FUNCTION_PARAM_ADDRESS] < 210 )
	{
		String_Debug_Once(SCRIPT_FUNCTION_DEBUG,p_magnetic_entry,2,(40,"pen far away from magnetic\r"));
		MagneticStateSet(X_False);
	}
	else
	{
		p_SFP ->buf_8[MAGNETIC_FUNCTION_PARAM_ADDRESS] = 0;
	}
	return X_True;
}
static X_Boolean QuickPenDistanceFromMagneticGenerator(X_Void * p_param)
{
	uint8_t random_num;
	ScriptFunctionParam * p_SFP;

	p_SFP = (ScriptFunctionParam *)p_param;
	random_num = p_SFP ->buf_8[CURRENT_RANDOM_NUM_ADDRESS];

	// get app state

	if(random_num % 4 == 0 )
	{
		String_Debug_Once(SCRIPT_FUNCTION_DEBUG,p_magnetic_entry,1,(30,"pen near magnetic--\r"));
		MagneticStateSet(X_True);
	}
	else
	{
		String_Debug_Once(SCRIPT_FUNCTION_DEBUG,p_magnetic_entry,2,(40,"pen far away from magnetic--\r"));
		MagneticStateSet(X_False);
	}
	return X_True;
}
static X_Boolean PenPowerStateGenerator(X_Void * p_param)
{
	uint8_t random_num;
	ScriptFunctionParam * p_SFP;

	p_SFP = (ScriptFunctionParam *)p_param;
	random_num = p_SFP ->buf_8[CURRENT_RANDOM_NUM_ADDRESS];

	// get app state

	if(random_num <= 2 )
	{
		String_Debug_Once(SCRIPT_FUNCTION_DEBUG,p_power_entry,1,(30,"pen power extrlow\r"));
		PowerStateSet(X_True);
	}
	else
	{
		String_Debug_Once(SCRIPT_FUNCTION_DEBUG,p_power_entry,2,(30,"pen power normal\r"));
		PowerStateSet(X_False);
	}
	return X_True;
}
static X_Boolean QuickPenPowerStateGenerator(X_Void * p_param)
{
	uint8_t random_num;
	ScriptFunctionParam * p_SFP;

	p_SFP = (ScriptFunctionParam *)p_param;
	random_num = p_SFP ->buf_8[CURRENT_RANDOM_NUM_ADDRESS];

	// get app state
	if(DoesSureShutDown_TB() == X_True)
	{
		return X_True;
	}

	p_SFP ->buf_8[POWER_FUNCTION_PARAM_ADDRESS] = p_SFP ->buf_8[POWER_FUNCTION_PARAM_ADDRESS] + random_num;
	if(p_SFP ->buf_8[POWER_FUNCTION_PARAM_ADDRESS] >= 100 && p_SFP ->buf_8[POWER_FUNCTION_PARAM_ADDRESS] <= 120 )
	{
		String_Debug_Once(SCRIPT_FUNCTION_DEBUG,p_power_entry,1,(30,"pen power extrlow-- \r"));
		PowerStateSet(X_True);
	}
	else if (p_SFP ->buf_8[POWER_FUNCTION_PARAM_ADDRESS] < 100 )
	{
		String_Debug_Once(SCRIPT_FUNCTION_DEBUG,p_power_entry,2,(30,"pen power normal-- \r"));
		PowerStateSet(X_False);
	}
	else
	{
		p_SFP ->buf_8[POWER_FUNCTION_PARAM_ADDRESS] = 0;
	}
	return X_True;
}
static X_Boolean PowerOff(X_Void * p_param)
{

//	ScriptFunctionParam * p_SFP;
//
//	p_SFP = (ScriptFunctionParam *)p_param;

	String_Debug(SCRIPT_FUNCTION_DEBUG,(30,"power off\r"));
	ClearShutDownState_TB();
	ClearQuietState_TB();
	UserShutDownStateSet(X_False);
	OnceEntryValueInit(p_magnetic_entry,100);
	OnceEntryValueInit(p_power_entry,100);
	return X_True;
}
static X_Boolean StartAgain(X_Void * p_param)
{
	String_Debug(SCRIPT_FUNCTION_DEBUG,(30,"start again ====\r"));
	// sent app state
	PenMoveStateInit();
	BleStateInit();
	UserShutDownStateInit();
	UserWakeButtonStateInit();
//	OnceEntryValueInit(p_move_entry,100);
//	OnceEntryValueInit(p_ble_entry,100);
//	OnceEntryValueInit(p_magnetic_entry,100);
//	OnceEntryValueInit(p_power_entry,100);

	AllPeripheralInit();
	return X_True;
}
static X_Boolean BackToStart(X_Void * p_param)
{
	return X_True;
}

static X_Boolean DoesPowerOff(X_Void * p_param)
{
	if(DoesSureQuiet_TB() == X_True || DoesSureShutDown_TB() == X_True)
	{
		return X_True;
	}
	return X_False;
}

static struct _ScriptsFunctionArray
{
	uint8_t function_number;
	X_Boolean(*action)(X_Void * p_param);
}
const ScriptsFunctionArray[] = {
	  {1,ScriptsFunctionInitial},
	  {2,ForNow},
	  {3,LoopSpecificTimes},
	  {4,RandomNumberGenerator},
	  {5,UserShutDownGenerator},
	  {6,BleStateGenerator},
	  {7,PenMoveStateGenerator},
	  {8,PenChargeStateGenerator},
	  {9,PenDistanceFromMagneticGenerator},
	  {0x0a,PowerOff},
	  {0x0b,StartAgain},
	  {0x0c,PenPowerStateGenerator},
	  {0x0d,UserAppLock},
	  {0x0e,UserAppUnlock},
	  {0x0f,BackToStart},
	  {0x10,QuickPenChargeStateGenerator},
	  {0x11,QuickPenDistanceFromMagneticGenerator},
	  {0x12,QuickPenPowerStateGenerator},
	  {0x13,DoesPowerOff},
};

X_Boolean CallFunction(uint8_t func_num,X_Void * p_param)
{
	uint8_t i;
	X_Boolean isOK;

	isOK = X_False;
	for(i=0;i<sizeof(ScriptsFunctionArray)/sizeof(ScriptsFunctionArray[0]);i++)
	{
		if(ScriptsFunctionArray[i].function_number == func_num && ScriptsFunctionArray[i].action != X_Null)
		{
			isOK = ScriptsFunctionArray[i].action(p_param);
			break;
		}
	}
	return isOK;
}

/*
 *	1	:ScriptsFunctionInitial					(1)  00010100
 *	2	:ForNow									(2)  00010100
 *	3	:UserAppLock							(0d) 00010100
 *	4	:StartAgain								(0b) 00010100
 *	5	:RandomNumberGenerator					(4)  00010114
 *	6	:UserShutDownGenerator					(5)  00010100
 *	7	:PenMoveStateGenerator					(7)  00010100
 *	8	:BleStateGenerator						(6)  00010100
 *	9	:PenChargeStateGenerator				(8)  00010164
 *	a	:PenPowerStateGenerator					(0c) 00010100
 *	b	:PenDistanceFromMagneticGenerator		(9)  00010100
 *	c	:DoesPowerOff							(13) 010e0d00
 *	d	:UserAppUnlock							(0e) 01050d00
 *	e	:UserAppLock							(0d) 00010100
 *	f	:PowerOff								(0a) 00010100
 *	10	:RandomNumberGenerator					(4)  0001010a
 *	11	:QuickPenChargeStateGenerator			(10) 00010107
 *	12	:QuickPenDistanceFromMagneticGenerator	(11) 00010100
 *	13	:QuickPenPowerStateGenerator			(12) 00010100
 *	14	:LoopSpecificTimes						(3)  01151405
 *	15	:UserAppUnlock							(0e) 01041500
 *
1:command:07550100010100
2:command:07550200010100
3:command:07550d00010100
4:command:07550b00010100
5:command:07550400010114
6:command:07550500010100
7:command:07550700010100
8:command:07550600010100
9:command:07550800010164
a:command:07550c00010100
b:command:07550900010100
c:command:075513010e0d00
d:command:07550e01050d00
e:command:07550d00010100
f:command:07550a00010100
10:command:07550400010107
11:command:07551000010100
12:command:07551100010100
13:command:07551200010100
14:command:07550301151405
15:command:07550e01041500
16:end
 */
static X_Boolean isBatteryCharge;

static X_Void BatteryChargeInit(X_Void)
{
	isBatteryCharge = X_False;
}

static X_Boolean BatteryChargeStateToggle(X_Void)
{
	if(isBatteryCharge == X_True) {isBatteryCharge = X_False;}
	else {isBatteryCharge = X_True;}
	return isBatteryCharge;
}

X_Boolean BatteryIsChargerConnected(X_Void)
{
	return isBatteryCharge;
}

static X_Boolean isPenMove;
static X_Void PenMoveStateInit(X_Void)
{
	isPenMove = X_True;
}
static X_Void PenMoveStateSet(X_Boolean isMove)
{
	isPenMove = isMove;
}
X_Boolean PenMoveStateGet(X_Void)
{
	return isPenMove;
}

static X_Boolean isBleConnected;
static X_Void BleStateInit(X_Void)
{
	isBleConnected = X_False;
}
static X_Void BleStateSet(X_Boolean isOK)
{
	isBleConnected = isOK;
}
X_Boolean BleStateGet(X_Void)
{
	return isBleConnected;
}

static X_Boolean isMagneticNear;
static X_Void MagneticStateInit(X_Void)
{
	isMagneticNear = X_False;
}
static X_Void MagneticStateSet(X_Boolean isOK)
{
	isMagneticNear = isOK;
}
X_Boolean MagneticStateGet(X_Void)
{
	return isMagneticNear;
}

static X_Boolean isPowerLow;
static X_Void PowerStateInit(X_Void)
{
	isPowerLow = X_False;
}
static X_Void PowerStateSet(X_Boolean isOK)
{
	isPowerLow = isOK;
}
X_Boolean PowerStateGet(X_Void)
{
	return isPowerLow;
}

static X_Boolean isUserShutDown;
static X_Void UserShutDownStateInit(X_Void)
{
	isUserShutDown = X_False;
}
static X_Void UserShutDownStateSet(X_Boolean isOK)
{
	isUserShutDown = isOK;
}
X_Boolean UserShutDownStateGet(X_Void)
{
	return isUserShutDown;
}

static X_Boolean isWakeUpButtonPushed;
static X_Void UserWakeButtonStateInit(X_Void)
{
	isWakeUpButtonPushed = X_False;
}
static X_Void UserWakeButtonStateSet(X_Boolean isOK)
{
	isWakeUpButtonPushed = isOK;
}
X_Boolean UserWakeButtonStateGet(X_Void)
{
	return isWakeUpButtonPushed;
}
