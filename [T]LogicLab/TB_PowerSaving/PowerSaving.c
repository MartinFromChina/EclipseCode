#include "PowerSaving.h"
#include "PenState.h"
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

static X_Boolean ScriptsFunctionInitial(X_Void * p_param)
{
	uint8_t i;
	String_Debug(SCRIPT_FUNCTION_DEBUG,(30,"1 :initial is called\r\n"));

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

	String_Debug(SCRIPT_FUNCTION_DEBUG,(30,"2 : ForNow is called \r\n"));

	BatteryChargeInit();
	return X_True;
}
static X_Boolean LoopSpecificTimes(X_Void * p_param)
{
	ScriptFunctionParam * p_SFP;
	p_SFP = (ScriptFunctionParam *)p_param;

//	String_Debug(SCRIPT_FUNCTION_DEBUG,(30,"3 : Loop is called : %d\r\n",p_SFP ->buf_8[0]));
	p_SFP ->isParamValid = X_True;
	if(p_SFP ->buf_8[LOOP_COUNTER_ADDRESS]+1 >= p_SFP ->buf_8[LOOP_COUNTER_THRESHOLD_ADDRESS])
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

	if(random_num == 20 )
	{
		String_Debug(SCRIPT_FUNCTION_DEBUG,(30,"user shut down the pen\r\n"));
		p_SFP ->buf_8[SECOND_CONDITION_JUMP_ADDRESS] = 1;
	}
	else
	{
		p_SFP ->buf_8[SECOND_CONDITION_JUMP_ADDRESS] = 0;
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

	if(p_SFP ->buf_8[SECOND_CONDITION_JUMP_ADDRESS] == 1)
	{
		String_Debug_Once(SCRIPT_FUNCTION_DEBUG,p_ble_entry,2,(30,"ble disconnected \r\n"));
	}
	else
	{
		p_SFP ->buf_8[BLE_FUNCTION_PARAM_ADDRESS] = p_SFP ->buf_8[BLE_FUNCTION_PARAM_ADDRESS] + random_num;
		if(p_SFP ->buf_8[BLE_FUNCTION_PARAM_ADDRESS] > 40 && p_SFP ->buf_8[BLE_FUNCTION_PARAM_ADDRESS] < 80)
		{
			String_Debug_Once(SCRIPT_FUNCTION_DEBUG,p_ble_entry,1,(30,"ble connected \r\n"));
		}
		else if(p_SFP ->buf_8[BLE_FUNCTION_PARAM_ADDRESS] <= 80)
		{

			String_Debug_Once(SCRIPT_FUNCTION_DEBUG,p_ble_entry,2,(30,"ble disconnected \r\n"));
		}
		else
		{
			p_SFP ->buf_8[BLE_FUNCTION_PARAM_ADDRESS] = 0;
		}
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

	if(random_num % 5 == 0 || random_num % 3 == 0)
	{
		String_Debug_Once(SCRIPT_FUNCTION_DEBUG,p_move_entry,1,(30,"pen silence\r\n"));
	}
	else if(random_num % 4 == 0 || random_num % 6 == 0)
	{
		String_Debug_Once(SCRIPT_FUNCTION_DEBUG,p_move_entry,2,(30,"pen move\r\n"));
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
static X_Boolean PenDistanceFromMagneticGenerator(X_Void * p_param)
{
	uint8_t random_num;
	ScriptFunctionParam * p_SFP;

	p_SFP = (ScriptFunctionParam *)p_param;
	random_num = p_SFP ->buf_8[CURRENT_RANDOM_NUM_ADDRESS];

	// get app state

	p_SFP ->buf_8[MAGNETIC_FUNCTION_PARAM_ADDRESS] = p_SFP ->buf_8[MAGNETIC_FUNCTION_PARAM_ADDRESS] + random_num;
	if(p_SFP ->buf_8[MAGNETIC_FUNCTION_PARAM_ADDRESS] >= 60 && p_SFP ->buf_8[MAGNETIC_FUNCTION_PARAM_ADDRESS] <= 100 )
	{
		String_Debug_Once(SCRIPT_FUNCTION_DEBUG,p_magnetic_entry,1,(30,"pen near magnetic\r\n"));
	}
	else if (p_SFP ->buf_8[MAGNETIC_FUNCTION_PARAM_ADDRESS] < 60 )
	{
		String_Debug_Once(SCRIPT_FUNCTION_DEBUG,p_magnetic_entry,2,(30,"pen far away from magnetic\r\n"));
	}
	else
	{
		p_SFP ->buf_8[MAGNETIC_FUNCTION_PARAM_ADDRESS] = 0;
	}

	if(p_SFP ->buf_8[SECOND_CONDITION_JUMP_ADDRESS] == 1)
	{
		return X_False;
	}
	else
	{
		return X_True;
	}
}
static X_Boolean PenPowerStateGenerator(X_Void * p_param)
{
	uint8_t random_num;
	ScriptFunctionParam * p_SFP;

	p_SFP = (ScriptFunctionParam *)p_param;
	random_num = p_SFP ->buf_8[CURRENT_RANDOM_NUM_ADDRESS];

	// get app state

	p_SFP ->buf_8[POWER_FUNCTION_PARAM_ADDRESS] = p_SFP ->buf_8[POWER_FUNCTION_PARAM_ADDRESS] + random_num;
	if(p_SFP ->buf_8[POWER_FUNCTION_PARAM_ADDRESS] >= 100 && p_SFP ->buf_8[POWER_FUNCTION_PARAM_ADDRESS] <= 120 )
	{
		String_Debug_Once(SCRIPT_FUNCTION_DEBUG,p_power_entry,1,(30,"pen power extrlow\r\n"));
	}
	else if (p_SFP ->buf_8[POWER_FUNCTION_PARAM_ADDRESS] < 100 )
	{
		String_Debug_Once(SCRIPT_FUNCTION_DEBUG,p_power_entry,2,(30,"pen power normal\r\n"));
	}
	else
	{
		p_SFP ->buf_8[POWER_FUNCTION_PARAM_ADDRESS] = 0;
	}
	return X_True;
}

static X_Boolean PowerOff(X_Void * p_param)
{

	ScriptFunctionParam * p_SFP;

	p_SFP = (ScriptFunctionParam *)p_param;
    p_SFP ->buf_8[LOOP_COUNTER_THRESHOLD_ADDRESS] = p_SFP->param_from_script;

	String_Debug(SCRIPT_FUNCTION_DEBUG,(30,"power off\r\n"));
	return X_True;
}
static X_Boolean StartAgain(X_Void * p_param)
{
	String_Debug(SCRIPT_FUNCTION_DEBUG,(30,"start again\r\n"));
	// sent app state
	AllPeripheralInit();
	return X_True;
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
 *	1	:ScriptsFunctionInitial  00
 *	2	:ForNow 00
 *	3	:StartAgain 00
 *	4	:RandomNumberGenerator 00
 *	5	:UserShutDownGenerator 00
 *	6	:PenMoveStateGenerator 00
 *	7	:BleStateGenerator 00
 *	8	:PenChargeStateGenerator 00 xx xx 64
 *	9	:PenPowerStateGenerator 00
 *	a	:PenDistanceFromMagneticGenerator 01 04 0b
 *	b	:PowerOff 00 xx xx 0a
 *	c	:LoopSpecificTimes 01 03 0c
 *
1:command:07550100010100
2:command:07550200010100
3:command:07550b00010100
4:command:07550400010114
5:command:07550500010100
6:command:07550700010100
7:command:07550600010100
8:command:07550800010164
9:command:07550c00010100
a:command:07550901040b00
b:command:07550a0001010a
c:command:07550301030c00
d:end
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
