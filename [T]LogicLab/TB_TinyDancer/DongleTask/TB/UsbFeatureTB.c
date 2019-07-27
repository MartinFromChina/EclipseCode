#include "UsbFeatureTB.h"
#include "DongleTaskTB.h"
#include "..\..\..\UserDebug.h"
#include "..\..\..\..\CommonSource\StateMachine\StateMachine.h"
#include "..\..\..\..\CommonSource\AppCommon.h"
#include "..\..\..\..\CommonSource\Math\random_number.h"
#include "..\DataFlow.h"

static uint8_t get_buf[MAX_DATA_LENGTH]= {0x02,0x01};
static uint8_t set_buf[MAX_DATA_LENGTH]= {0x03,0x00,0x00,0x00};

static _sCommandMap const CommandMap[]= {
		{0,SetTestMode			,3		,"SetTestMode"				},//3
		{1,SetReportBleState	,3		,"SetReportBleState"  	 	},//3
		{2,SetReportProductType	,3		,"SetReportProductType"		},//3
		{3,SetReportInformation	,4		,"SetReportInformation"		},//4
		{4,SetKeyValueMap		,15		,"SetKeyValueMap"			},//15
		{5,SetClearWhiteList	,3		,"SetClearWhiteList"		},//3
		{6,SetUserID			,13		,"SetUserID"				},//13
		{7,	SetPenType			,5		,"SetPenType"				},//5
		{8,	SetColor			,6		,"SetColor"					},//6
		{9,SetEnableHidDevice	,4		,"SetEnableHidDevice"		},//4

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
		current_num = GetRandomNumber(0,12);
		if(current_num > 9) {current_num = 0;}
		SEGGER_RTT_Debug(USB_FEATURE_DEBUG,(43,"SetFeatureAction %s\r\n",CommandMap[current_num % 10].p_char));

		set_buf[0] = CommandMap[current_num % 10].command_length;
		set_buf[1] = CommandMap[current_num % 10].command;

		DataFlowPush(CommandSetEntry,set_buf,set_buf[0]);
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
	SEGGER_RTT_Debug(USB_FEATURE_DEBUG,(30,"GetFeatureAction\r\n"));
	DataFlowPush(CommandGetEntry,get_buf,get_buf[0]);
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
