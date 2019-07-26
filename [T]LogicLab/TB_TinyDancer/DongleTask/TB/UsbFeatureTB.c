#include "UsbFeatureTB.h"
#include "DongleTaskTB.h"
#include "..\..\..\UserDebug.h"
#include "..\..\..\..\CommonSource\StateMachine\StateMachine.h"

static _sCommandMap const CommandMap[]= {
		{0,SetTestMode			,"SetTestMode"				},
		{1,SetReportBleState	,"SetReportBleState"  	 	},
		{2,SetReportProductType	,"SetReportProductType"		},
		{3,SetReportInformation	,"SetReportInformation"		},
		{4,SetKeyValueMap		,"SetKeyValueMap"			},
		{5,SetClearWhiteList	,"SetClearWhiteList"		},
		{6,SetUserID			,"SetUserID"				},
		{7,	SetPenType			,"SetPenType"				},
		{8,	SetColor			,"SetColor"					},
		{9,SetEnableHidDevice	,"SetEnableHidDevice"		},

};

typedef enum
{
	SetFeature = 0,
	FeatureDelay,
	GetFeature,
}UsbFeatureState;

static uint32_t current_num = 0;
static uint8_t DelayCounter = 0;


static StateNumber SetFeatureAction(StateNumber current_state)
{
	current_num = GetCurrentRandomNumber();
	if(current_num >= 15) {DelayCounter = current_num - 14;}
	else {DelayCounter = 0;}

	if(current_num % 3 == 0)
	{
		SEGGER_RTT_Debug(USB_FEATURE_DEBUG,(43,"SetFeatureAction %s\r\n",CommandMap[current_num % 10].p_char));
		if(DelayCounter == 0) {return GetFeature;}
		else {return FeatureDelay;}
	}
	else {return SetFeature;}
}
static StateNumber FeatureDelayAction(StateNumber current_state)
{
	if(DelayCounter > 0) {DelayCounter --;}
	if(DelayCounter > 0) {return FeatureDelay;}
	else {return GetFeature;}
}
static StateNumber GetFeatureAction(StateNumber current_state)
{
	SEGGER_RTT_Debug(DONGLE_SCRIPT_DEBUG,(30,"GetFeatureAction\r\n"));
	return SetFeature;
}

static const StateAction FeatureStateAction[3] = {
		{SetFeatureAction},
		{FeatureDelayAction},
		{GetFeatureAction},
};

APP_SIMPLE_STATE_MACHINE_DEF(p_feature_state,3,1,&FeatureStateAction[0]);

X_Void GetFeatureAndSetFeature(X_Void)
{
	SimpleStateMachineRun(p_feature_state,X_Null,X_Null);
}
