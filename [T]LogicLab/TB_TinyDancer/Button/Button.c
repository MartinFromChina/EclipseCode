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


X_Void onTick(X_Void)
{
	ButtonStateHandle();
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
