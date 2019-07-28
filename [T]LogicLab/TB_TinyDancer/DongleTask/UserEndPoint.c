#include "UserEndPoint.h"
#include "DataFlow.h"
#include "..\..\UserDebug.h"
#include "..\..\..\CommonSource\StateMachine\StateMachine.h"
typedef enum
{
	UE_Idle = 0,
	UE_CheckAirMouseNotify,
	UE_CheckKeyNotify,
	UE_CheckPenTipNotify,
	UE_CheckOtherNotify,
	UE_TheEnd,
}UserEndpointState;

#define HID_MOUSE_ENABLE  		 1
#define HID_KEYBOARD_ENABLE 	 1

static X_Boolean isBreak = X_False;
static uint8_t *p_data;
static uint8_t length = 0;
static uint8_t fast_notify_entry_counter = 0;

static StateNumber IdleAction(StateNumber current_state)
{
	isBreak = X_False;
	length = 0;
	fast_notify_entry_counter = 0;
	return (current_state + 1);
}
static StateNumber CheckAirMouseNotifyAction(StateNumber current_state)
{
	isBreak = X_False;
	if(DataFlowPop(CharAirMouseEntry,&p_data,&length) == X_True)
	{
		if(HID_MOUSE_ENABLE == 1)
		{
			SEGGER_RTT_Debug(USB_EP3_DEBUG,(40,"sent raw data to HID MOUSE:%d ;%2x %2x\r\n"
											,length,p_data[0],p_data[1]));
		}
		SEGGER_RTT_Debug(USB_EP3_DEBUG,(40,"sent airmouse data to ep3 :%d ;%2x %2x\r\n"
													,length,p_data[0],p_data[1]));

		DataFlowReleaseCurrentBuf(CharAirMouseEntry);
		isBreak = X_True;
		fast_notify_entry_counter ++;
	}
	return (current_state + 1);
}
static StateNumber CheckKeyNotifyAction(StateNumber current_state)
{
	isBreak = X_False;
	if(DataFlowPop(CharKeyEntry,&p_data,&length) == X_True)
	{

		if(HID_KEYBOARD_ENABLE == 1)
		{
			SEGGER_RTT_Debug(USB_EP3_DEBUG,(40,"sent raw data to HID KEYBOARD:%d ;%2x %2x\r\n"
											,length,p_data[0],p_data[1]));
		}
		SEGGER_RTT_Debug(USB_EP3_DEBUG,(40,"sent keyvalue data to ep3 :%d ;%2x %2x\r\n"
													,length,p_data[0],p_data[1]));

		DataFlowReleaseCurrentBuf(CharKeyEntry);

		isBreak = X_True;
		fast_notify_entry_counter ++;
	}
	return (current_state + 1);
}
static StateNumber CheckPenTipNotifyAction(StateNumber current_state)
{
	isBreak = X_False;
	if(DataFlowPop(CharPenTipEntry,&p_data,&length) == X_True)
	{

		SEGGER_RTT_Debug(USB_EP3_DEBUG,(40,"sent pentip data to ep3 :%d ;%2x %2x\r\n"
															,length,p_data[0],p_data[1]));
		DataFlowReleaseCurrentBuf(CharPenTipEntry);
		return UE_TheEnd;
	}
	else
	{
		if(fast_notify_entry_counter > 0)
		{
			return UE_TheEnd;
		}
		return (current_state + 1);
	}
}
static StateNumber CheckOtherNotifyAction(StateNumber current_state)
{
	isBreak = X_False;
	if(DataFlowPop(CharColorEntry,&p_data,&length) == X_True)
	{
		SEGGER_RTT_Debug(USB_EP3_DEBUG,(40,"sent color data to ep3 :%d ;%2x %2x\r\n"
																	,length,p_data[0],p_data[1]));
		DataFlowReleaseCurrentBuf(CharColorEntry);
	}
	else if (DataFlowPop(CharPenTypeEntry,&p_data,&length) == X_True)
	{
		SEGGER_RTT_Debug(USB_EP3_DEBUG,(40,"sent pentype data to ep3 :%d ;%2x %2x\r\n"
																	,length,p_data[0],p_data[1]));
		DataFlowReleaseCurrentBuf(CharPenTypeEntry);
	}
	else if (DataFlowPop(CharBatteryEntry,&p_data,&length) == X_True)
	{
		SEGGER_RTT_Debug(USB_EP3_DEBUG,(40,"sent battery data to ep3 :%d ;%2x %2x\r\n"
																	,length,p_data[0],p_data[1]));
		DataFlowReleaseCurrentBuf(CharBatteryEntry);
	}
	return UE_TheEnd;
}
static StateNumber TheEndAction(StateNumber current_state)
{
	isBreak = X_True;
	return UE_Idle;
}

static const StateAction EndPointStateAction[] = {
		{IdleAction},
		{CheckAirMouseNotifyAction},
		{CheckKeyNotifyAction},
		{CheckPenTipNotifyAction},
		{CheckOtherNotifyAction},
		{TheEndAction},
};

APP_SIMPLE_STATE_MACHINE_DEF(p_ep_state
							,sizeof(EndPointStateAction)/sizeof(EndPointStateAction[0])
							,5
							,&EndPointStateAction[0]);


static X_Boolean DoesBreakSimple(const StateSimpleParam *p_sbp,StateNumber nextstate,uint16_t loop_counter)
{
	if((*p_sbp ->p_CurrentStateNum) == UE_TheEnd) {return X_True;}
	return X_False;
}
static X_Void StateJumpRecorder(StateNumber current_state,StateNumber next_state)
{
	if(current_state == UE_TheEnd)
	{
		SEGGER_RTT_Debug(USB_EP3_DEBUG,(43,"/*****************************/ \r\n"));
	}
//	SEGGER_RTT_Debug(USB_EP3_DEBUG,(40,"usb_endpoint3 state :%d\r\n",current_state));
}
X_Boolean UserEndPointThreeHandle(X_Void)
{
	SimpleStateMachineRun(p_ep_state,DoesBreakSimple,StateJumpRecorder);
	return isBreak ;
}
