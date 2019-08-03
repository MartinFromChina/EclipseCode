
//#include "app_timer.h"
//
#include "DongleTask.h"
#include "DataFlow.h"
#include "..\..\UserDebug.h"
#include "UserEndPoint.h"
//#include "button.h"
//#include "user_usb.h"
//
//APP_TIMER_DEF(main_tick);
//
////RTT_DEBUG_COUNTER_ENTRY_DEF(p_button,0);
//
///**@brief Function for initializing the timer.
// */
//X_Void timer_init(X_Void)
//{
//    ret_code_t err_code = app_timer_init();
//    APP_ERROR_CHECK(err_code);
//}
//
//static void onTick(void * p_context)
//{
//	X_Boolean isChanged,isOK;
//	uint8_t button_value;
//	AllButtonAction(&isOK,&isChanged,&button_value);
//	UsbBehaviorHandle();
//
////	SEGGER_RTT_Debug_Counter(BUTTON_DOING_DEBUG,p_button,125,("button_value %2x\r\n",button_value));
//}
//X_Void StartTaskTimer(X_Void)
//{
//	app_timer_start(main_tick, APP_TIMER_TICKS(DONGLE_TASK_INTERVAL_IN_MS),X_Null);
//}

static X_Boolean isInit = X_False;
//static uint8_t *p_data;
//static uint8_t length;

X_Void DongleTaskInit(X_Void)
{
//	ButtonInitialize();
	DataFlowInit();
//	BasicUsbInit();
//	app_timer_create(&main_tick, APP_TIMER_MODE_REPEATED, onTick);
	isInit = X_True;
}

static X_Void AllEventHandle(X_Void);

X_Void onTick(X_Void)
{
	if(isInit == X_False) {return;}
		AllEventHandle();// USB event  ;  BLE event  ;   user event
}

/* call it three times faster than notify ,so that data buf will not be overwriten
  consider the worst situation :
  key ,air mouse ,pentip notify come together , function :AllEventHandle have to sent them out in 7.5ms
  which means 2.5ms for each notify in USB Interrupt endpoint 3 channel.
  if not , the queue buf could be full , there is a risk of buf be overwrite by higher interrupt when lower interrupt is using it

  also ,there is another way: set the buf flag: occupy not permit  so that the overwriten push will be failed,the latest data will be lost

 */

static X_Void AllEventHandle(X_Void)
{
	uint8_t i;
	X_Boolean isBreak;
	/*
	for(i = 0;i<EntryMaxNumber;i++)
	{
		if(DataFlowPop((DataFlowEntry)i,&p_data,&length) == X_True)
		{
			SEGGER_RTT_Debug((DATA_FLOW_DEBUG && (GetDebugFlag(i) == 1)),(USER_MAX_STRING_LENGTH," %s :data:%2x %2x \r\n"
												,EventStringGet(i),p_data[0],p_data[1]));
			DataFlowReleaseCurrentBuf((DataFlowEntry)i);
		}
	}
	*/
	for(i = 0;i<3 ;i++ ) // call it three times so that
	{
		isBreak = UserEndPointThreeHandle();
		if(isBreak == X_True)
		{
			break;
		}
	}
	//HidMouseAndKeyBoardHandle();
//	UsbSetAndGetFeatureHandle();
}


