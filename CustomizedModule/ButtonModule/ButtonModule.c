#include "ButtonModule.h"
#include "..\..\CommonSource\Math\bit_operation.h"

#define TIMER_COUNTER_MAX                      (0xffff)

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
	BM_Init = 0,
	BM_Start,
	BM_ClickDetect,
	BM_LongPushDetect,
	BM_SureLongPush,
	BM_LongPushReleaseDetect,
	BM_SureLongPushRelease,
	BM_ClickReleaseDetect,
	BM_DoubleClickDetect,
	BM_SureClick,
}ButtonMonitorState;

typedef struct
{
	uint32_t isClick;
	uint32_t isContinusClick;
	uint32_t isDoubleClick;
	uint32_t isLongpush;
	uint32_t isLongpushRelease;
}sButtonParam;

static X_Boolean isCurrentButtonPushed = X_False;
static CombineButtonValue CurrentButtonValue = 0;
static uint8_t CurrentButtonNumber = 0;
static CombineButtonValue CurrentButtonConfigMode = 0;
static sParamSingleButton sPSB[MAX_BUTTON_NUMBER];
static const sButtonModuleExtern *p_sBME;

static const sParamAboutTime sPAT_Default = {
		ClickTimeLimitInMsDefault,
		ContinuousClickTimeLimitInMsDefault,
		RealseTimeLimitInMsDefault,
		DoubleClickTimeIntervalInMsDefault,
		LongPushTimeLimitInMsDefault,
};

static X_Void TimeManager(sParamSingleButton * p_psb,X_Boolean isPushed)
{
	if(p_psb == X_Null) {return;}

	if(isPushed == X_True)
	{
		p_psb->release_time_counter = 0;
		if(p_psb->push_time_counter >= TIMER_COUNTER_MAX) {return;}
		p_psb->push_time_counter ++;

	}
	else
	{
		p_psb->push_time_counter = 0;
		if(p_psb->release_time_counter >= TIMER_COUNTER_MAX) {return;}
		p_psb->release_time_counter ++;
	}
}

StateNumber CustomizedBM_InitAction(StateNumber current_state)
{
	if(CurrentButtonNumber >= p_sBME->button_number) {return BM_Init;}
	if(p_sBME->base->config == X_Null)
	{
		sPSB[CurrentButtonNumber].p_spat = &sPAT_Default;
	}
	else
	{
		p_sBME->base->config(&sPSB[CurrentButtonNumber]);// to do :get something param from flash or set it into flash
	}
	sPSB[CurrentButtonNumber].CurrentOM = (twobyte_getbit(CurrentButtonConfigMode,CurrentButtonNumber) == 0 ) ? MouseMode : KeyBoardMode;
	sPSB[CurrentButtonNumber].push_time_counter = 0;
	sPSB[CurrentButtonNumber].release_time_counter = 0;
	return BM_Start;
}
StateNumber CustomizedBM_StartAction(StateNumber current_state)
{
//	if(CurrentButtonNumber >= p_sBME->button_number) {return BM_Init;}
//	sParamSingleButton * p_spsb = &sPSB[CurrentButtonNumber];
//	if((p_spsb->push_time_counter * p_sBME->base->ModuleLoopTimeInMS) >= p_spsb -> p_spat->ClickTimeThresholdInMS)
	return BM_ClickDetect;
}
StateNumber CustomizedBM_ClickDetectAction(StateNumber current_state)
{
	if(CurrentButtonNumber >= p_sBME->button_number) {return BM_Init;}
	sParamSingleButton * p_spsb = &sPSB[CurrentButtonNumber];
}
StateNumber CustomizedBM_LongPushDetectAction(StateNumber current_state)
{
	if(CurrentButtonNumber >= p_sBME->button_number) {return BM_Init;}
	sParamSingleButton * p_spsb = &sPSB[CurrentButtonNumber];
}
StateNumber CustomizedBM_SureLongPushAction(StateNumber current_state)
{
	if(CurrentButtonNumber >= p_sBME->button_number) {return BM_Init;}
	sParamSingleButton * p_spsb = &sPSB[CurrentButtonNumber];
}
StateNumber CustomizedBM_LongPushReleaseDetectAction(StateNumber current_state)
{
	if(CurrentButtonNumber >= p_sBME->button_number) {return BM_Init;}
	sParamSingleButton * p_spsb = &sPSB[CurrentButtonNumber];
}
StateNumber CustomizedBM_SureLongPushReleaseAction(StateNumber current_state)
{
	if(CurrentButtonNumber >= p_sBME->button_number) {return BM_Init;}
	sParamSingleButton * p_spsb = &sPSB[CurrentButtonNumber];
}
StateNumber CustomizedBM_ClickReleaseDetectAction(StateNumber current_state)
{
	if(CurrentButtonNumber >= p_sBME->button_number) {return BM_Init;}
	sParamSingleButton * p_spsb = &sPSB[CurrentButtonNumber];
}
StateNumber CustomizedBM_DoubleClickDetectAction(StateNumber current_state)
{
	if(CurrentButtonNumber >= p_sBME->button_number) {return BM_Init;}
	sParamSingleButton * p_spsb = &sPSB[CurrentButtonNumber];
}
StateNumber CustomizedBM_SureClickAction(StateNumber current_state)
{
	if(CurrentButtonNumber >= p_sBME->button_number) {return BM_Init;}
	sParamSingleButton * p_spsb = &sPSB[CurrentButtonNumber];
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
		TimeManager(&sPSB[CurrentButtonNumber],isCurrentButtonPushed);
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
