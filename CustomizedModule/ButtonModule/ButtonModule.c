#include "ButtonModule.h"
#include "..\..\CommonSource\Math\bit_operation.h"

#define TIMER_COUNTER_MAX                      (0xffff)

#define VALID_PUSH_IN_MS_DEFAULT                100
#define VALID_RELEASE_IN_MS_DEFAULT             40
#define VALID_LONG_PUSH_IN_MS_DEFAULT           1000
#define VALID_LONG_RELEASE_IN_MS_DEFAULT        80

#define SetBitMethod             twobyte_setbit
/*************************************************************************/
typedef enum
{
	BA_click,
	BA_doubleclick,
	BA_longpush,
	BA_longpushrelease,
}eButtonAction;

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
	BM_SureDoubleClick,
}ButtonMonitorState;

typedef struct
{
	CombineButtonValue isClick;
	CombineButtonValue isDoubleClick;
	CombineButtonValue isLongpush;
	CombineButtonValue isLongpushRelease;
	uint16_t LongPushLastCycle[MAX_BUTTON_NUMBER];
}sButtonStateFlag;

static X_Boolean isCurrentButtonPushed = X_False;
static CombineButtonValue CurrentButtonValue = 0;
static uint8_t CurrentButtonNumber = 0;
static sParamSingleButton sPSB[MAX_BUTTON_NUMBER];
static const sButtonModuleExtern *p_sBME;
static sButtonStateFlag sBSF = {0,0,0,0};

static const sParamAboutTime sPAT_Default = {
		VALID_PUSH_IN_MS_DEFAULT,
		VALID_RELEASE_IN_MS_DEFAULT,
		VALID_LONG_PUSH_IN_MS_DEFAULT,
		VALID_LONG_RELEASE_IN_MS_DEFAULT,
};

static X_Boolean DoesValidPush(const sParamSingleButton * p_psb)
{
	if(p_psb == X_Null || p_sBME == X_Null) {return X_False;}
	return ((p_psb->push_time_counter * p_sBME->base->ModuleLoopTimeInMS)  >=  p_psb->p_spat->ClickTimeThresholdInMS);
}
static X_Boolean DoesValidRelease(const sParamSingleButton * p_psb)
{
	if(p_psb == X_Null || p_sBME == X_Null) {return X_False;}
	return ((p_psb->release_time_counter * p_sBME->base->ModuleLoopTimeInMS)  >=  p_psb->p_spat->ReleaseTimeThresholdInMS);
}
static X_Boolean DoesValidLongPush(const sParamSingleButton * p_psb)
{
	if(p_psb == X_Null || p_sBME == X_Null) {return X_False;}
	return ((p_psb->push_time_counter * p_sBME->base->ModuleLoopTimeInMS)  >=  p_psb->p_spat->LongPushTimeThresholdInMS);
}
static X_Boolean DoesValidLongRelease(const sParamSingleButton * p_psb)
{
	if(p_psb == X_Null || p_sBME == X_Null) {return X_False;}
	return ((p_psb->release_time_counter * p_sBME->base->ModuleLoopTimeInMS)  >=  p_psb->p_spat->ReleaseAllTimeThresholdInMS);
}


static X_Void ClearPushCounter(sParamSingleButton * p_psb)
{
	if(p_psb == X_Null) {return ;}
	p_psb->latest_push_time_counter_backup = p_psb->push_time_counter;
	p_psb->push_time_counter = 0;
}
static X_Void ClearReleaseCounter(sParamSingleButton * p_psb)
{
	if(p_psb == X_Null) {return ;}
	p_psb->release_time_counter = 0;
}

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
		if(p_psb->push_time_counter != 0) {p_psb->latest_push_time_counter_backup = p_psb->push_time_counter;}
		p_psb->push_time_counter = 0;
		if(p_psb->release_time_counter >= TIMER_COUNTER_MAX) {return;}
		p_psb->release_time_counter ++;
	}
}



static X_Void ButtonFlagInit(X_Void)
{
	sBSF.isClick = 0;
	sBSF.isDoubleClick = 0;
	sBSF.isLongpush = 0;
	sBSF.isLongpushRelease = 0;
}

static X_Void ButtonFlagSet(eButtonAction action,uint8_t button_num,uint16_t long_push_last)
{
	switch(action)
	{
		case BA_click:
			sBSF.isClick = SetBitMethod(sBSF.isClick,button_num);
			break;
		case BA_doubleclick:
			sBSF.isDoubleClick = SetBitMethod(sBSF.isDoubleClick,button_num);
			break;
		case BA_longpush:
			sBSF.isLongpush = SetBitMethod(sBSF.isLongpush,button_num);
			break;
		case BA_longpushrelease:
			sBSF.isLongpushRelease = SetBitMethod(sBSF.isLongpushRelease,button_num);
			if(button_num >= MAX_BUTTON_NUMBER) {break;}
			sBSF.LongPushLastCycle[button_num] = long_push_last;
			break;
		default:
			break;
	}
}
static X_Void ButtonFlagReport(const sButtonModuleExtern *p_sbm)
{
	if(p_sbm->base->click != X_Null && sBSF.isClick != 0)
	{
		p_sbm->base->click(sBSF.isClick);
	}
	if(p_sbm->base->double_click != X_Null && sBSF.isDoubleClick != 0)
	{
		p_sbm->base->double_click(sBSF.isDoubleClick);
	}
	if(p_sbm->base->long_push != X_Null && sBSF.isLongpush != 0)
	{
		p_sbm->base->long_push(sBSF.isLongpush);
	}
	if(p_sbm->base->long_push_release != X_Null && sBSF.isLongpushRelease != 0)
	{
		p_sbm->base->long_push_release(sBSF.isLongpushRelease,sBSF.LongPushLastCycle);
	}
	ButtonFlagInit();
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
	sPSB[CurrentButtonNumber].push_time_counter = 0;
	sPSB[CurrentButtonNumber].release_time_counter = 0;
	sPSB[CurrentButtonNumber].latest_push_time_counter_backup = 0;
	ButtonFlagInit();
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
	if(DoesValidPush(&sPSB[CurrentButtonNumber]) == X_True)
	{
		ClearPushCounter(&sPSB[CurrentButtonNumber]);
		return BM_LongPushDetect;
	}
	if(DoesValidLongRelease(&sPSB[CurrentButtonNumber]) == X_True) {return BM_Start;}
	return current_state;
}
StateNumber CustomizedBM_LongPushDetectAction(StateNumber current_state)
{
	if(CurrentButtonNumber >= p_sBME->button_number) {return BM_Init;}
	if(DoesValidLongPush(&sPSB[CurrentButtonNumber]) == X_True) {return BM_SureLongPush;}
	if(DoesValidRelease(&sPSB[CurrentButtonNumber]) == X_True)
	{
		ClearReleaseCounter(&sPSB[CurrentButtonNumber]);
		return BM_ClickReleaseDetect;
	}
	return current_state;
}
StateNumber CustomizedBM_SureLongPushAction(StateNumber current_state)
{
	// to do : tell user
	ButtonFlagSet(BA_longpush,CurrentButtonNumber,0);
	return BM_LongPushReleaseDetect;
}
StateNumber CustomizedBM_LongPushReleaseDetectAction(StateNumber current_state)
{
	if(CurrentButtonNumber >= p_sBME->button_number) {return BM_Init;}
	if(DoesValidRelease(&sPSB[CurrentButtonNumber]) == X_True) {return BM_SureLongPushRelease;}
	return current_state;
}
StateNumber CustomizedBM_SureLongPushReleaseAction(StateNumber current_state)
{
	if(CurrentButtonNumber >= p_sBME->button_number) {return BM_Init;}
	// to do : tell user
	ButtonFlagSet(BA_longpushrelease,CurrentButtonNumber,sPSB[CurrentButtonNumber].latest_push_time_counter_backup);
	return BM_Start;
}
StateNumber CustomizedBM_ClickReleaseDetectAction(StateNumber current_state)
{
	if(CurrentButtonNumber >= p_sBME->button_number) {return BM_Init;}
	if(DoesValidLongRelease(&sPSB[CurrentButtonNumber]) == X_True) {return BM_SureClick;}
	if(DoesValidPush(&sPSB[CurrentButtonNumber]) == X_True)
	{
		ClearPushCounter(&sPSB[CurrentButtonNumber]);
		return BM_DoubleClickDetect;
	}
	return current_state;
}
StateNumber CustomizedBM_DoubleClickDetectAction(StateNumber current_state)
{
	if(CurrentButtonNumber >= p_sBME->button_number) {return BM_Init;}
	if(DoesValidRelease(&sPSB[CurrentButtonNumber]) == X_True) {return BM_SureDoubleClick;}
	if(DoesValidLongPush(&sPSB[CurrentButtonNumber]) == X_True){return BM_SureLongPush;}
	return current_state;
}
StateNumber CustomizedBM_SureClickAction(StateNumber current_state)
{
	// to do : tell user
	ButtonFlagSet(BA_click,CurrentButtonNumber,0);
	return BM_Start;
}
StateNumber CustomizedBM_SureDoubleClickAction(StateNumber current_state)
{
	// to do : tell user
	ButtonFlagSet(BA_doubleclick,CurrentButtonNumber,0);
	return BM_Start;
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
		SimpleStateMachineRun(p_sbm->p_monitor[i],X_Null,p_sbm->base->StateRecorder);
		TimeManager(&sPSB[CurrentButtonNumber],isCurrentButtonPushed);
	}
	ButtonFlagReport(p_sbm);
}

uint8_t GetCurrentButtonNumber(X_Void)
{
	return CurrentButtonNumber;
}
