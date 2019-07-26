#include "NotifyEventTB.h"
#include "DongleTaskTB.h"
#include "..\..\..\UserDebug.h"
#include "..\..\..\..\CommonSource\StateMachine\StateMachine.h"

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
static uint32_t current_num = 0;
static sNotifyEvent sNE;

static _sCommandMap const CommandMap[]= {
		{0,KeyNotify		,"KeyNotify"		},
		{1,AirMouseNotify	,"AirMouseNotify"  	},
		{2,PenTipNotify		,"PenTipNotify"		},
		{3,ColorNotify		,"ColorNotify"		},
		{4,PenTypeNotify	,"PenTypeNotify"	},
		{5,BatteryNotify	,"BatteryNotify"	},
};

typedef enum
{
	Idle = 0,
	ConditionDetected,
}NotityEventState;


static StateNumber IdleAction(StateNumber current_state)
{
	SEGGER_RTT_Debug(NOTITY_DEBUG,(30,"----NotityEventInit\r\n"));
	current_num = 0;
	isBleConnect = X_False;
	sNE.isAirMouseNotify 	= 0;
	sNE.isBatteryNotify 	= 0;
	sNE.isColorNotify 		= 0;
	sNE.isKeyNotify 		= 0;
	sNE.isPenTipNotify 		= 0;
	sNE.isPenTypeNotify 	= 0;

	return ConditionDetected;
}
static StateNumber ConditionDetectedAction(StateNumber current_state)
{
	if(isBleConnect == X_False)
	{
		SEGGER_RTT_Debug_Once(NOTITY_DEBUG,p_notify,1,(30,"!!! no notify\r\n"));
		return current_state;
	}
	SEGGER_RTT_Debug_Once(NOTITY_DEBUG,p_notify,2,(30,"!!!  notify\r\n"));

	SEGGER_RTT_Debug_Once(NOTITY_DEBUG,p_key		,sNE.isKeyNotify		,(30,"!!!  key_notify %d\r\n",sNE.isKeyNotify));
	SEGGER_RTT_Debug_Once(NOTITY_DEBUG,p_airmouse	,sNE.isAirMouseNotify	,(30,"!!!  AirMouse_notify %d\r\n",sNE.isAirMouseNotify));
	SEGGER_RTT_Debug_Once(NOTITY_DEBUG,p_pentip		,sNE.isPenTipNotify		,(30,"!!!  PenTip_notify %d\r\n",sNE.isPenTipNotify));
	SEGGER_RTT_Debug_Once(NOTITY_DEBUG,p_color		,sNE.isColorNotify		,(30,"!!!  Color_notify %d\r\n",sNE.isColorNotify));
	SEGGER_RTT_Debug_Once(NOTITY_DEBUG,p_pentype	,sNE.isPenTypeNotify	,(30,"!!!  PenType_notify %d\r\n",sNE.isPenTypeNotify));
	SEGGER_RTT_Debug_Once(NOTITY_DEBUG,p_battery	,sNE.isBatteryNotify	,(30,"!!!  Battery_notify %d\r\n",sNE.isBatteryNotify));

	current_num = GetCurrentRandomNumber();

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
