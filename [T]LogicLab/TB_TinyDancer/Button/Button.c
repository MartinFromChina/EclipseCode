#include "Button.h"
#include "ButtonTB.h"
#include "..\..\..\CommonSource\AppCommon.h"
#include "..\..\..\CommonSource\StateMachine\StateMachine.h"
#include "..\..\..\CommonSource\Math\bit_operation.h"
#include "..\..\..\CustomizedModule\ButtonModule\ButtonModule.h"

#define UserNeedLongPush

RTT_DEBUG_ONCE_ENTRY_DEF(p_button,0);

/*****************************long push*******************************/
#ifdef UserNeedLongPush
static void Button1_LongPushDoing(void)
{
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,(30,"Button Zero LongPush \r\n"));
}
static void Button2_LongPushDoing(void)
{
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,(30,"Button One LongPush \r\n"));
}
static void Button3_LongPushDoing(void)
{
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,(30,"Button Two LongPush \r\n"));
}
static void Button4_LongPushDoing(void)
{
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,(30,"Button Three LongPush \r\n"));
}
static void Button5_LongPushDoing(void)
{
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,(30,"Button Four LongPush \r\n"));
}
static void Button6_LongPushDoing(void)
{
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,(30,"Button Five LongPush \r\n"));
}

static struct _LongPushDoing
{
	void (*Action)(void);
}
const LongPushDoing[]={
	{Button1_LongPushDoing},
	{Button2_LongPushDoing},
	{Button3_LongPushDoing},
	{Button4_LongPushDoing},
	{Button5_LongPushDoing},
	{Button6_LongPushDoing},
};

static void LongPushBehavior(const CombineButtonValue value)
{
	uint8_t i;
	SEGGER_RTT_Debug(BUTTON_BEHAVIOR_DEBUG,(30,"LongPush : %2x\r\n",value));
	for(i=0;i<sizeof(LongPushDoing)/sizeof(LongPushDoing[0]);i++)
	{
		if((value & (ButtonMask << i)) != 0)
		{
			if(LongPushDoing[i].Action != X_Null)
			{
				LongPushDoing[i].Action();
			}
		}
	}
}

#endif

/*****************************long push rerlease*******************************/
#ifdef UserNeedLongPushRelease
static void Button2_LongPushReleaseDoing(uint16_t tickcyle)
{
	#if (UseButtonBehaviorAnalyze == 1)
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,("Button One LongPush Release; last %d ms : %d + %d\r\n"
										,((tickcyle*ButtonFunctionCallFrequencyInMs)+GetButtonActionParam(LongPushBeginParam))
										,GetButtonActionParam(LongPushBeginParam)
										,(tickcyle*ButtonFunctionCallFrequencyInMs)));
	#endif
}
static void Button3_LongPushReleaseDoing(uint16_t tickcyle)
{
	#if (UseButtonBehaviorAnalyze == 1)
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,("Button Two LongPush Release; last %d ms : %d + %d\r\n"
										,((tickcyle*ButtonFunctionCallFrequencyInMs)+GetButtonActionParam(LongPushBeginParam))
										,GetButtonActionParam(LongPushBeginParam)
										,(tickcyle*ButtonFunctionCallFrequencyInMs)));
	#endif
}
static void Button4_LongPushReleaseDoing(uint16_t tickcyle)
{
	#if (UseButtonBehaviorAnalyze == 1)
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,("Button Three LongPush Release; last %d ms : %d + %d\r\n"
										,((tickcyle*ButtonFunctionCallFrequencyInMs)+GetButtonActionParam(LongPushBeginParam))
										,GetButtonActionParam(LongPushBeginParam)
										,(tickcyle*ButtonFunctionCallFrequencyInMs)));
	#endif
}
static void Button5_LongPushReleaseDoing(uint16_t tickcyle)
{
	#if (UseButtonBehaviorAnalyze == 1)
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,("Button Four LongPush Release; last %d ms : %d + %d\r\n"
										,((tickcyle*ButtonFunctionCallFrequencyInMs)+GetButtonActionParam(LongPushBeginParam))
										,GetButtonActionParam(LongPushBeginParam)
										,(tickcyle*ButtonFunctionCallFrequencyInMs)));
	#endif
}
static void Button6_LongPushReleaseDoing(uint16_t tickcyle)
{
	#if (UseButtonBehaviorAnalyze == 1)
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,("Button Five LongPush Release; last %d ms : %d + %d\r\n"
										,((tickcyle*ButtonFunctionCallFrequencyInMs)+GetButtonActionParam(LongPushBeginParam))
										,GetButtonActionParam(LongPushBeginParam)
										,(tickcyle*ButtonFunctionCallFrequencyInMs)));
	PowerKeyLongPushStop();
	#endif
}

static struct _LongPushReleaseDoing
{
	void (*Action)(uint16_t tickcyle);
}
const LongPushReleaseDoing[]={
	{X_Null},
	{Button2_LongPushReleaseDoing},
	{Button3_LongPushReleaseDoing},
	{Button4_LongPushReleaseDoing},
	{Button5_LongPushReleaseDoing},
	{Button6_LongPushReleaseDoing},
};

static void LongPushFinishedBehavior(CombineButtonValue value,uint16_t const*longpushtickcycle)
{
	uint8_t i;
	SEGGER_RTT_Debug(BUTTON_BEHAVIOR_DEBUG,("LongPushRelease : %2x\r\n",value));

	for(i=0;i<sizeof(LongPushReleaseDoing)/sizeof(LongPushReleaseDoing[0]);i++)
	{
		if((value & (ButtonMask << i)) != 0)
		{
			if(LongPushReleaseDoing[i].Action != X_Null)
			{
				LongPushReleaseDoing[i].Action(longpushtickcycle[i]);
			}
		}
	}

}
#endif

/***************************** click*******************************/
#ifdef UserNeedClick

static void Button2_ContinuClickDoing(void)
{
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,("Button One Maybe Continu Click \r\n"));
////	SetButtonAction(ColorChange);
}
static void Button3_ContinuClickDoing(void)
{
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,("Button Two Maybe Continu Click \r\n"));
}
static void Button4_ContinuClickDoing(void)
{
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,("Button Three Maybe Continu Click \r\n"));
}
static void Button5_ContinuClickDoing(void)
{
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,("Button Four Maybe Continu Click \r\n"));
}
static void Button6_ContinuClickDoing(void)
{
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,("Button Five Maybe Continu Click \r\n"));
}

static struct _ContinuClickDoing
{
	void (*Action)(void);
}
const ContinuClickDoing[]={
	{X_Null},
	{Button2_ContinuClickDoing},
	{Button3_ContinuClickDoing},
	{Button4_ContinuClickDoing},
	{Button5_ContinuClickDoing},
	{Button6_ContinuClickDoing},
};
static void MaybeContinuClickBehavior(CombineButtonValue value)
{
	uint8_t i;
	SEGGER_RTT_Debug(BUTTON_BEHAVIOR_DEBUG,("ContinuClick : %2x\r\n",value));

	for(i=0;i<sizeof(ContinuClickDoing)/sizeof(ContinuClickDoing[0]);i++)
	{
		if((value & (ButtonMask << i)) != 0)
		{
			if(ContinuClickDoing[i].Action != X_Null)
			{
				ContinuClickDoing[i].Action();
			}
		}
	}
}


static void Button2_ClickDoing(void)
{
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,("Button One Click \r\n"));
}
static void Button3_ClickDoing(void)
{
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,("Button Two Click \r\n"));
}
static void Button4_ClickDoing(void)
{
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,("Button Three Click \r\n"));
}
static void Button5_ClickDoing(void)
{
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,("Button Four Click \r\n"));
}
static void Button6_ClickDoing(void)
{
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,("Button Five Click \r\n"));
}


static struct _ClickDoing
{
	void (*Action)(void);
}
const ClickDoing[]={
	{X_Null},
	{Button2_ClickDoing},
	{Button3_ClickDoing},
	{Button4_ClickDoing},
	{Button5_ClickDoing},
	{Button6_ClickDoing},
};
static void ClickBehavior(CombineButtonValue value)
{
	uint8_t i;
	SEGGER_RTT_Debug(BUTTON_BEHAVIOR_DEBUG,("Click : %2x\r\n",value));

	for(i=0;i<sizeof(ClickDoing)/sizeof(ClickDoing[0]);i++)
	{
		if((value & (ButtonMask << i)) != 0)
		{
			if(ClickDoing[i].Action != X_Null)
			{
				ClickDoing[i].Action();
			}
		}
	}
}
#endif

/*****************************double click*******************************/
#ifdef UserNeedDoubleClick
static void Button2_DoubleClickDoing(void)
{
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,("Button One Double Click \r\n"));
}
static void Button3_DoubleClickDoing(void)
{
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,("Button Two Double Click \r\n"));
}
static void Button4_DoubleClickDoing(void)
{
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,("Button Three Double Click \r\n"));
}
static void Button5_DoubleClickDoing(void)
{
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,("Button Four Double Click \r\n"));
}
static void Button6_DoubleClickDoing(void)
{
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,("Button Five Double Click \r\n"));
}
static struct _DoubleClickDoing
{
	void (*Action)(void);
}
const DoubleClickDoing[]={
	{X_Null},
	{Button2_DoubleClickDoing},
	{Button3_DoubleClickDoing},
	{Button4_DoubleClickDoing},
	{Button5_DoubleClickDoing},
	{Button6_DoubleClickDoing},
};

static void DoubleClickBehavior(CombineButtonValue value)
{
	uint8_t i;
	SEGGER_RTT_Debug(BUTTON_BEHAVIOR_DEBUG,("DoubleClick : %2x\r\n",value));

	for(i=0;i<sizeof(DoubleClickDoing)/sizeof(DoubleClickDoing[0]);i++)
	{
		if((value & (ButtonMask << i)) != 0)
		{
			if(DoubleClickDoing[i].Action != X_Null)
			{
				DoubleClickDoing[i].Action();
			}
		}
	}
}
#endif


FILE * ButtonTestOpenFile(void)
{
	return fopen(".//TB_TinyDancer//Button//command.txt", "r");
}

X_Void ButtonInitialize(X_Void)
{

}
static X_Boolean ButtonStateHandle();






/*
 * *************************************************************************************************
 * state machine test
 * typedef struct
{
	X_Boolean (*onHop)(X_Void);
	StateNumber NextStateWhenTrue;
	StateNumber NextStateWhenFalse;
}sHopHandle;

typedef struct {
	uint8_t      current_hop_times;
	sHopHandle * p_hop;
}StateHandle;

typedef struct
{
	const StateNumber 	AllStateNum;
	const uint16_t     	MaxStateHopTimesInSignalProcess;
	StateHandle const 	*p_Handle;
	StateNumber 		*p_CurrentStateNum;
}StateBasicParam;
 * *************************************************************************************************
 */
static X_Boolean  DoesIdleHop1(s_StateMachineParam *p_this)
{
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,(30,"DoesIdleHop1 \r\n"));
	return X_True;
}
static X_Boolean DoesIdleHop2(s_StateMachineParam *p_this)
{
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,(30,"DoesIdleHop2 \r\n"));
	return X_True;
}
static X_Boolean DoesIdleHop3(s_StateMachineParam *p_this)
{
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,(30,"DoesIdleHop3 \r\n"));
	return X_False;
}
static X_Boolean DoesIdleHop4(s_StateMachineParam *p_this)
{
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,(30,"DoesIdleHop4 \r\n"));
	return X_True;
}
static X_Boolean DoesSecondHop2(s_StateMachineParam *p_this)
{
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,(30,"DoesSecondHop2 \r\n"));
	return X_True;
}
static const sHopHandle IdleHop[] =
{
		{X_Null,CURRENT_STATE_FLAG,1},
		{DoesIdleHop2,200,1},
		{DoesIdleHop3,CURRENT_STATE_FLAG,100},
		{DoesIdleHop4,CURRENT_STATE_FLAG,1},
};

static const sHopHandle SecondHop[] =
{
		{X_Null,0,1},
		{DoesSecondHop2,0,1},
		{X_Null,0,1},
};


static const StateHandle ExampleStateHandle[] = {
		{0,sizeof(IdleHop)/sizeof(IdleHop[0])		,&IdleHop[0]},
		{1,sizeof(SecondHop)/sizeof(SecondHop[0])	,&SecondHop[0]},
		{2,0										,X_Null},
};

APP_STATE_MACHINE_DEF(p_state_machine
						,sizeof(ExampleStateHandle)/sizeof(ExampleStateHandle[0])
						,2
						,&ExampleStateHandle[0]);



static X_Boolean DoesBreak(const StateBasicParam *p_sbp,StateNumber nextstate,uint16_t loop_counter)
{
	if(loop_counter >= 2) {return X_True;}
	return X_False;
}

static X_Void StateJumpRecorder(StateNumber state_going_to_leave,StateNumber state_going_to_come)
{
	// going to jump new state:next_state
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,(40," leave %d come %d \r\n",state_going_to_leave,state_going_to_come));
}

static s_StateMachineParam s_SMP;

X_Void onTick(X_Void)
{
	uint32_t error;
	error = StateMachineRun(p_state_machine,&s_SMP,DoesBreak,StateJumpRecorder);
	SEGGER_RTT_Debug(BUTTON_DOING_DEBUG,(40," **************error: %d\r\n",error));
//	ButtonStateHandle();
}

/*
,function_init
,function_get_value
,function_config
,function_click_cb
,function_continus_click_cb
,function_double_click_cb
,fucntion_long_push_cb
,function_long_push_release_cb
*/

CUSTOM_BUTTON_MONITOR_2_DEF(p_monitor
						   ,3
						   ,X_Null
						   ,GetCurrentButtonValue
						   ,X_Null
						   ,X_Null
						   ,X_Null
						   ,LongPushBehavior
						   ,X_Null
						   ,X_Null);



static X_Boolean ButtonStateHandle(X_Void)
{
	CombineButtonValue  value;
	ButtonStateMonitor(p_monitor,&value);
	SEGGER_RTT_Debug_Once(BUTTON_SCRIPT_DEBUG,p_button,value,(40,"button : %s ,%d\r\n"
												,ValueToBinaryString(X_True,8,value),value));
	return X_True;
}
