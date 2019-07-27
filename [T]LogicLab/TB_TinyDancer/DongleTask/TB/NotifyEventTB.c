#include "NotifyEventTB.h"
#include "DongleTaskTB.h"
#include "..\..\..\UserDebug.h"
#include "..\..\..\..\CommonSource\StateMachine\StateMachine.h"
#include "..\..\..\..\CommonSource\Math\bit_operation.h"
#include "..\..\..\..\CommonSource\Math\random_number.h"
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

static uint8_t get_buf[MAX_DATA_LENGTH];
static uint8_t set_buf[MAX_DATA_LENGTH];

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

static X_Void BufInit(uint8_t *p_buf,uint8_t length,uint8_t init_value)
{
	uint8_t i;
	for(i = 0;i<length;i++)
	{
		p_buf[i] = init_value;
	}
}

static uint8_t get_buf[MAX_DATA_LENGTH];
static uint8_t set_buf[MAX_DATA_LENGTH];

static uint8_t key_buf[CHAR_KEY_LENGTH];
static uint8_t pentip_buf[CHAR_PENTIP_LENGTH];
static uint8_t airmouse_buf[CHAR_AIRMOUSE_LENGTH];
static uint8_t color_buf[CHAR_COLOR_LENGTH];
static uint8_t pentype_buf[CHAR_PENTYPE_LENGTH];
static uint8_t battery_buf[CHAR_BATTERY_LENGTH];

static X_Void AllBufInit(X_Void)
{
	BufInit(get_buf			,MAX_DATA_LENGTH		,0);
	BufInit(set_buf			,MAX_DATA_LENGTH		,0);
	BufInit(key_buf			,CHAR_KEY_LENGTH		,0);
	BufInit(pentip_buf		,CHAR_PENTIP_LENGTH		,0);
	BufInit(airmouse_buf	,CHAR_AIRMOUSE_LENGTH	,0);
	BufInit(color_buf		,CHAR_COLOR_LENGTH		,0);
	BufInit(pentype_buf		,CHAR_PENTYPE_LENGTH	,0);
	BufInit(battery_buf		,CHAR_BATTERY_LENGTH	,0);
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
		airmouse_buf[0] = airmouse_buf[0] + 1;
		DataFlowPush(CharAirMouseEntry,airmouse_buf,CHAR_AIRMOUSE_LENGTH);
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
	AirMouseNotifyHandle();

	if(delay_counter >= 8)
	{
		delay_counter = 0;

		current_num = GetRandomNumber(0,63);
		SEGGER_RTT_Debug_Once(NOTITY_DEBUG,p_notify,2,(40,"--------------  notify %2x\r\n",current_num));
		NotityEventUpData(current_num);

		SEGGER_RTT_Debug_Once(NOTITY_DEBUG,p_key		,sNE.isKeyNotify		,(30,"!!!  key_notify %d\r\n",sNE.isKeyNotify));

		SEGGER_RTT_Debug_Once(NOTITY_DEBUG,p_pentip		,sNE.isPenTipNotify		,(30,"!!!  PenTip_notify %d\r\n",sNE.isPenTipNotify));
		SEGGER_RTT_Debug_Once(NOTITY_DEBUG,p_color		,sNE.isColorNotify		,(30,"!!!  Color_notify %d\r\n",sNE.isColorNotify));
		SEGGER_RTT_Debug_Once(NOTITY_DEBUG,p_pentype	,sNE.isPenTypeNotify	,(30,"!!!  PenType_notify %d\r\n",sNE.isPenTypeNotify));
		SEGGER_RTT_Debug_Once(NOTITY_DEBUG,p_battery	,sNE.isBatteryNotify	,(30,"!!!  Battery_notify %d\r\n",sNE.isBatteryNotify));
	}
	else
	{
		delay_counter ++;
	}
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
