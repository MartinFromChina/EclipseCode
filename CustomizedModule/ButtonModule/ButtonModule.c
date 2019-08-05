#include "ButtonModule.h"
#include "..\..\CommonSource\Math\bit_operation.h"

#define NewStartTimeLimitBig    				3000 							//ms
#define NewStartTimeLimitSmall  				(ButtonSampleFrequencyInMs*2)  	//48   //ms

#define ClickTimeLimitInMsDefault   			400								//ms
#define ContinuousClickTimeLimitInMsDefault   	5								//ms
#define RealseTimeLimitInMsDefault   			40								//ms
#define DoubleClickTimeIntervalInMsDefault   	120								//ms
#define LongPushTimeLimitInMsDefault   			900								//ms
/*************************************************************************/
typedef enum
{
	MouseMode = 0, // click 	,double click 		, long push
	KeyBoardMode ,  // click 	, continous click 	, long push
}ButtonOperationMode;
typedef struct
{
	uint32_t isClick;
	uint32_t isContinusClick;
	uint32_t isDoubleClick;
	uint32_t isLongpush;
	uint32_t isLongpushRelease;
}sButtonParam;
typedef struct
{
	uint16_t ClickTimeThresholdInMS;
	uint16_t ContinuousClickTimeThresholdInMS;
	uint16_t ReleaseTimeThresholdInMS;
	uint16_t DoubleClickTimeIntervalThresholdInMS;
	uint16_t LongPushTimeThresholdInMS;
}sParamAboutTime;

static X_Boolean isCurrentButtonPushed = X_False;
static CombineButtonValue CurrentButtonValue = 0;
static uint8_t CurrentButtonNumber = 0;
static CombineButtonValue CurrentButtonConfigMode = 0;
static sParamSingleButton sPSB[MAX_BUTTON_NUMBER];
static sButtonModuleExtern *p_sBME;

static const sParamAboutTime sPAT_Default = {
		ClickTimeLimitInMsDefault,
		ContinuousClickTimeLimitInMsDefault,
		RealseTimeLimitInMsDefault,
		DoubleClickTimeIntervalInMsDefault,
		LongPushTimeLimitInMsDefault,
};


StateNumber CustomizedBM_InitAction(StateNumber current_state)
{
	if(p_sBME->base->config == X_Null)
	{
		sPSB[CurrentButtonNumber].p_spat = &sPAT_Default;
	}
	else
	{
		p_sBME->base->config(&sPSB[CurrentButtonNumber]);// to do :get something param from flash or set it into flash
	}
	sPSB[CurrentButtonNumber].CurrentOM = (twobyte_getbit(CurrentButtonConfigMode,CurrentButtonNumber) == 0 ) ? MouseMode : KeyBoardMode;
	return current_state;
}

StateNumber CustomizedBM_InitAction(StateNumber current_state)
{

}


X_Void ButtonStateMonitor(const sButtonModuleExtern *p_sbm,CombineButtonValue *value)
{
	uint8_t i;

	CurrentButtonValue = p_sbm->base->get_value();
	*value = CurrentButtonValue;
	p_sBME = p_sbm;
	for(i=0;i<p_sbm->button_number;i++)
	{
		CurrentButtonNumber = i;
		isCurrentButtonPushed = (twobyte_getbit(CurrentButtonValue,CurrentButtonNumber) == 1) ? X_True : X_False;
		SimpleStateMachineRun(p_sbm->p_monitor[i],X_Null,X_Null);
	}
}
X_Void SetCurrentButtonConfigMode(CombineButtonValue mode)
{
	CurrentButtonConfigMode = mode;
}
/*
#define FourByteMask							((uint32_t)(1u << 0))
static uint8_t byte4_getbit(uint32_t source,uint8_t bitnumber)
{
	if((source & (FourByteMask << bitnumber)) != 0)
	{
		return 1;
	}
	return 0;
}*/
