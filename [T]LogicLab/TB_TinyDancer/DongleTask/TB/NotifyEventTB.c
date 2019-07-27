#include "NotifyEventTB.h"
#include "DongleTaskTB.h"
#include "..\..\..\UserDebug.h"
#include "..\..\..\..\CommonSource\StateMachine\StateMachine.h"
#include "..\..\..\..\CommonSource\Math\bit_operation.h"
#include "..\..\..\..\CommonSource\Math\random_number.h"
#include "..\..\..\..\CommonSource\AppCommon.h"
#include "..\DataFlow.h"

typedef enum
{
	KeyNotify = 0,
	AirMouseNotify,
	PenTipNotify,
	ColorNotify,
	PenTypeNotify,
	BatteryNotify,
}EventCharacteristic;

typedef struct
{
	uint8_t isKeyNotify 		:1;
	uint8_t isAirMouseNotify	:1;
	uint8_t	isPenTipNotify		:1;
	uint8_t	isColorNotify		:1;
	uint8_t	isPenTypeNotify		:1;
	uint8_t	isBatteryNotify		:1;
}sNotifyEvent;

RTT_DEBUG_ONCE_ENTRY_DEF(p_notify,100);
RTT_DEBUG_ONCE_ENTRY_DEF(p_key,100);
RTT_DEBUG_ONCE_ENTRY_DEF(p_airmouse,100);
RTT_DEBUG_ONCE_ENTRY_DEF(p_pentip,100);
RTT_DEBUG_ONCE_ENTRY_DEF(p_color,100);
RTT_DEBUG_ONCE_ENTRY_DEF(p_pentype,100);
RTT_DEBUG_ONCE_ENTRY_DEF(p_battery,100);




static X_Boolean isBleConnect = X_False;
static uint8_t current_num = 0,delay_counter = 0;
static sNotifyEvent sNE;

static uint8_t key_buf[CHAR_KEY_LENGTH];
static uint8_t pentip_buf[CHAR_PENTIP_LENGTH];
static uint8_t airmouse_buf[CHAR_AIRMOUSE_LENGTH];
static uint8_t color_buf[CHAR_COLOR_LENGTH];
static uint8_t pentype_buf[CHAR_PENTYPE_LENGTH];
static uint8_t battery_buf[CHAR_BATTERY_LENGTH];

typedef enum
{
	Idle = 0,
	ConditionDetected,
}NotityEventState;

static X_Void NotityEventReset(X_Void)
{
	sNE.isAirMouseNotify 	= 0;
	sNE.isBatteryNotify 	= 0;
	sNE.isColorNotify 		= 0;
	sNE.isKeyNotify 		= 0;
	sNE.isPenTipNotify 		= 0;
	sNE.isPenTypeNotify 	= 0;
}

static X_Void NotityEventDebugReset(X_Void)
{
	*p_key 		= 0;
	*p_airmouse = 0;
	*p_pentip 	= 0;
	*p_color 	= 0;
	*p_pentype 	= 0;
	*p_battery  = 0;
}

static X_Void AllBufInit(X_Void)
{
	byteBufInit(key_buf			,CHAR_KEY_LENGTH		,0);
	byteBufInit(pentip_buf		,CHAR_PENTIP_LENGTH		,0);
	byteBufInit(airmouse_buf	,CHAR_AIRMOUSE_LENGTH	,0);
	byteBufInit(color_buf		,CHAR_COLOR_LENGTH		,0);
	byteBufInit(pentype_buf		,CHAR_PENTYPE_LENGTH	,0);
	byteBufInit(battery_buf		,CHAR_BATTERY_LENGTH	,0);
}

static X_Void NotityEventUpData(uint8_t random_num)
{
	sNE.isKeyNotify	 		= byte_getbit(random_num,0) ;
	sNE.isAirMouseNotify 	= byte_getbit(random_num,1) ;
	sNE.isPenTipNotify 		= byte_getbit(random_num,2) ;
	sNE.isColorNotify 		= byte_getbit(random_num,3) ;
	sNE.isPenTypeNotify 	= byte_getbit(random_num,4) ;
	sNE.isBatteryNotify 	= byte_getbit(random_num,5) ;
}

static X_Void AirMouseNotifyHandle(X_Void)
{
	SEGGER_RTT_Debug_Once(NOTITY_DEBUG,p_airmouse	,sNE.isAirMouseNotify	,(30,"!!!  AirMouse_notify %d\r\n",sNE.isAirMouseNotify));
	if(isBleConnect == X_False)
	{
		DataFlowClear(CharAirMouseEntry);
		return;
	}

	if(sNE.isAirMouseNotify == 1)
	{
		airmouse_buf[1] = airmouse_buf[1] + 3;
		airmouse_buf[4] = airmouse_buf[4] + 3;
		DataFlowPush(CharAirMouseEntry,airmouse_buf,CHAR_AIRMOUSE_LENGTH);
	}
}
static X_Void KeyNotifyHandle(X_Void)
{
	SEGGER_RTT_Debug_Once(NOTITY_DEBUG,p_key		,sNE.isKeyNotify		,(30,"!!!  key_notify %d\r\n",sNE.isKeyNotify));
	if(isBleConnect == X_False)
	{
		DataFlowClear(CharKeyEntry);
		return;
	}

	if(sNE.isKeyNotify == 1)
	{
		key_buf[0] = key_buf[0] + 1;
		DataFlowPush(CharKeyEntry,key_buf,CHAR_KEY_LENGTH);
	}
}
static X_Void PentipNotifyHandle(X_Void)
{
	SEGGER_RTT_Debug_Once(NOTITY_DEBUG,p_pentip		,sNE.isPenTipNotify		,(30,"!!!  PenTip_notify %d\r\n",sNE.isPenTipNotify));
	if(isBleConnect == X_False)
	{
		DataFlowClear(CharPenTipEntry);
		return;
	}

	if(sNE.isPenTipNotify == 1)
	{
		pentip_buf[1] = pentip_buf[1] + 1;
		pentip_buf[2] = pentip_buf[2] + 1;
		DataFlowPush(CharPenTipEntry,pentip_buf,CHAR_PENTIP_LENGTH);
	}
}
static X_Void ColorNotifyHandle(X_Void)
{
	SEGGER_RTT_Debug_Once(NOTITY_DEBUG,p_color		,sNE.isColorNotify		,(30,"!!!  Color_notify %d\r\n",sNE.isColorNotify));
	if(isBleConnect == X_False)
	{
		DataFlowClear(CharColorEntry);
		return;
	}

	if(sNE.isColorNotify == 1)
	{
		color_buf[0] = color_buf[0] + 0x55;
		color_buf[0] = color_buf[0] + 0x55;
		color_buf[0] = color_buf[0] + 0x55;
		DataFlowPush(CharColorEntry,color_buf,CHAR_COLOR_LENGTH);
	}
}
static X_Void PentypeNotifyHandle(X_Void)
{
	SEGGER_RTT_Debug_Once(NOTITY_DEBUG,p_pentype	,sNE.isPenTypeNotify	,(30,"!!!  PenType_notify %d\r\n",sNE.isPenTypeNotify));
	if(isBleConnect == X_False)
	{
		DataFlowClear(CharPenTypeEntry);
		return;
	}

	if(sNE.isPenTypeNotify == 1)
	{
		pentype_buf[0] = pentype_buf[0] + 1;
		DataFlowPush(CharPenTypeEntry,pentype_buf,CHAR_PENTYPE_LENGTH);
	}
}
static X_Void BatteryNotifyHandle(X_Void)
{
	SEGGER_RTT_Debug_Once(NOTITY_DEBUG,p_battery	,sNE.isBatteryNotify	,(30,"!!!  Battery_notify %d\r\n",sNE.isBatteryNotify));
	if(isBleConnect == X_False)
	{
		DataFlowClear(CharBatteryEntry);
		return;
	}

	if(sNE.isBatteryNotify == 1)
	{
		if(battery_buf[0] <= 100) {battery_buf[0] = battery_buf[0] + 1;}
		else {battery_buf[0] = 34;}

		DataFlowPush(CharBatteryEntry,battery_buf,CHAR_BATTERY_LENGTH);
	}
}

static StateNumber IdleAction(StateNumber current_state)
{
	SEGGER_RTT_Debug(NOTITY_DEBUG,(30,"----NotityEventInit\r\n"));
	current_num = 0;
	delay_counter = 8;
	isBleConnect = X_False;

	AllBufInit();
	NotityEventReset();

	return ConditionDetected;
}
static StateNumber ConditionDetectedAction(StateNumber current_state)
{
	if(isBleConnect == X_False)
	{
		SEGGER_RTT_Debug_Once(NOTITY_DEBUG,p_notify,1,(30,"!!! no notify\r\n"));
		NotityEventReset();
		NotityEventDebugReset();
		delay_counter = 7;
		return current_state;
	}


	if(delay_counter >= 8)
	{
		delay_counter = 0;

		current_num = GetRandomNumber(0,63);
		SEGGER_RTT_Debug_Once(NOTITY_DEBUG,p_notify,2,(40,"--------------  notify %2x\r\n",current_num));
		NotityEventUpData(current_num);
	}
	else
	{
		delay_counter ++;
	}

	AirMouseNotifyHandle();
	KeyNotifyHandle();
	PentipNotifyHandle();
	ColorNotifyHandle();
	PentypeNotifyHandle();
	BatteryNotifyHandle();

	return current_state;
}

static const StateAction SimpleStateAction[2] = {
		{IdleAction},
		{ConditionDetectedAction},
};

APP_SIMPLE_STATE_MACHINE_DEF(p_state,2,1,&SimpleStateAction[0]);

X_Void CharacteristicNotifyEventHandle(X_Boolean isConnected)
{
	isBleConnect = isConnected;
	SimpleStateMachineRun(p_state,X_Null,X_Null);
}

X_Void ResetNotifyEventState(X_Void)
{
	(*p_state->p_CurrentStateNum) = Idle;
}
