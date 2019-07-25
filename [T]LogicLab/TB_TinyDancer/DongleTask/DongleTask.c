
//#include "app_timer.h"
//
#include "DongleTask.h"
#include "DataFlow.h"
#include "..\..\UserDebug.h"
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

X_Void DongleTaskInit(X_Void)
{
//	ButtonInitialize();
	DataFlowInit();
//	BasicUsbInit();
//	app_timer_create(&main_tick, APP_TIMER_MODE_REPEATED, onTick);
}
X_Void onTick(X_Void)
{

}
