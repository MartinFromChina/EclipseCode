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
static const sButtonModuleExtern *p_sBME;
static sButtonStateFlag sBSF = {0,0,0,0};

static const sParamAboutTime sPAT_Default = {
		VALID_PUSH_IN_MS_DEFAULT,
		VALID_RELEASE_IN_MS_DEFAULT,
		VALID_LONG_PUSH_IN_MS_DEFAULT,
		VALID_LONG_RELEASE_IN_MS_DEFAULT,
};

typedef struct
{
	s_StateMachineParam base;
	uint8_t CurrentButtonNumber;
	sParamSingleButton s_psb;
}sParamExtern;

static sParamExtern sPE[MAX_BUTTON_NUMBER];

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

StateNumber CustomizedBM_InitAction(s_StateMachineParam *p_this)
{
	sParamExtern *p_spe = (sParamExtern*)p_this;
	if(p_sBME->base->config == X_Null)
	{
		p_spe->s_psb.p_spat = &sPAT_Default;
	}
	else
	{
		p_sBME->base->config(&p_spe->s_psb);// to do :get something param from flash or set it into flash
	}
	p_spe->s_psb.push_time_counter = 0;
	p_spe->s_psb.release_time_counter = 0;
	p_spe->s_psb.latest_push_time_counter_backup = 0;
	ButtonFlagInit();
	return BM_Start;
}
StateNumber CustomizedBM_StartAction(s_StateMachineParam *p_this)
{
//	if(CurrentButtonNumber >= p_sBME->button_number) {return BM_Init;}
//	sParamSingleButton * p_spsb = &sPSB[CurrentButtonNumber];
//	if((p_spsb->push_time_counter * p_sBME->base->ModuleLoopTimeInMS) >= p_spsb -> p_spat->ClickTimeThresholdInMS)
	return BM_ClickDetect;
}
StateNumber CustomizedBM_ClickDetectAction(s_StateMachineParam *p_this)
{
	sParamExtern *p_spe = (sParamExtern*)p_this;
	if(DoesValidPush(&p_spe->s_psb) == X_True)
	{
		ClearPushCounter(&p_spe->s_psb);
		return BM_LongPushDetect;
	}
	if(DoesValidLongRelease(&p_spe->s_psb) == X_True) {return BM_Start;}
	return p_this->current_state;
}
StateNumber CustomizedBM_LongPushDetectAction(s_StateMachineParam *p_this)
{
	sParamExtern *p_spe = (sParamExtern*)p_this;
	if(DoesValidLongPush(&p_spe->s_psb) == X_True) {return BM_SureLongPush;}
	if(DoesValidRelease(&p_spe->s_psb) == X_True)
	{
		ClearReleaseCounter(&p_spe->s_psb);
		return BM_ClickReleaseDetect;
	}
	return p_this->current_state;
}
StateNumber CustomizedBM_SureLongPushAction(s_StateMachineParam *p_this)
{
	// to do : tell user
	sParamExtern *p_spe = (sParamExtern*)p_this;
	ButtonFlagSet(BA_longpush,p_spe->CurrentButtonNumber,0);
	return BM_LongPushReleaseDetect;
}
StateNumber CustomizedBM_LongPushReleaseDetectAction(s_StateMachineParam *p_this)
{
	sParamExtern *p_spe = (sParamExtern*)p_this;
	if(p_spe->CurrentButtonNumber >= p_sBME->button_number) {return BM_Init;}
	if(DoesValidRelease(&p_spe->s_psb) == X_True) {return BM_SureLongPushRelease;}
	return p_this->current_state;
}
StateNumber CustomizedBM_SureLongPushReleaseAction(s_StateMachineParam *p_this)
{
	sParamExtern *p_spe = (sParamExtern*)p_this;
	if(p_spe->CurrentButtonNumber >= p_sBME->button_number) {return BM_Init;}
	// to do : tell user
	ButtonFlagSet(BA_longpushrelease,p_spe->CurrentButtonNumber,p_spe->s_psb.latest_push_time_counter_backup);
	return BM_Start;
}
StateNumber CustomizedBM_ClickReleaseDetectAction(s_StateMachineParam *p_this)
{
	sParamExtern *p_spe = (sParamExtern*)p_this;
	if(p_spe->CurrentButtonNumber >= p_sBME->button_number) {return BM_Init;}
	if(DoesValidLongRelease(&p_spe->s_psb) == X_True) {return BM_SureClick;}
	if(DoesValidPush(&p_spe->s_psb) == X_True)
	{
		ClearPushCounter(&p_spe->s_psb);
		return BM_DoubleClickDetect;
	}
	return p_this->current_state;
}
StateNumber CustomizedBM_DoubleClickDetectAction(s_StateMachineParam *p_this)
{
	sParamExtern *p_spe = (sParamExtern*)p_this;
	if(p_spe->CurrentButtonNumber >= p_sBME->button_number) {return BM_Init;}
	if(DoesValidRelease(&p_spe->s_psb) == X_True) {return BM_SureDoubleClick;}
	if(DoesValidLongPush(&p_spe->s_psb) == X_True){return BM_SureLongPush;}
	return p_this->current_state;
}
StateNumber CustomizedBM_SureClickAction(s_StateMachineParam *p_this)
{
	// to do : tell user
	sParamExtern *p_spe = (sParamExtern*)p_this;
	ButtonFlagSet(BA_click,p_spe->CurrentButtonNumber,0);
	return BM_Start;
}
StateNumber CustomizedBM_SureDoubleClickAction(s_StateMachineParam *p_this)
{
	// to do : tell user
	sParamExtern *p_spe = (sParamExtern*)p_this;
	ButtonFlagSet(BA_doubleclick,p_spe->CurrentButtonNumber,0);
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
		sPE[i].CurrentButtonNumber = i;
		isCurrentButtonPushed = (twobyte_getbit(CurrentButtonValue,i) == 1) ? X_True : X_False;
		SimpleStateMachineRun(p_sbm->p_monitor[i],&sPE[i].base,X_Null,p_sbm->base->StateRecorder);
		TimeManager(&sPE[i].s_psb,isCurrentButtonPushed);
	}
	ButtonFlagReport(p_sbm);
}
