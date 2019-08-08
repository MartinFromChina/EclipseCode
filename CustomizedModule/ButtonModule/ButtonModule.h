#ifndef __BUTTONMODULE_H
#define __BUTTONMODULE_H

#include "..\..\CommonSource\KeilMDK.h"
#include "..\..\CommonSource\CommonMarco.h"

#define  MAX_BUTTON_NUMBER   16
#define  CombineButtonValue  uint16_t
typedef uint32_t        			 ButtonActionFlagMask;
#define ButtonMask					 ((ButtonActionFlagMask)(1u << 0))

#define ButtonSampleFrequencyInMs        (24)
#define LongPushMonitorResetInterval     500 // tick cycle

#include "ButtonStateMonitor.h"

typedef enum
{
	MouseMode = 0, // click 	,double click 		, long push
	KeyBoardMode ,  // click 	, continous click 	, long push
}ButtonOperationMode;

typedef struct
{
	uint16_t ClickTimeThresholdInMS;
	uint16_t ContinuousClickTimeThresholdInMS;
	uint16_t ReleaseTimeThresholdInMS;
	uint16_t DoubleClickTimeIntervalThresholdInMS;
	uint16_t LongPushTimeThresholdInMS;
}sParamAboutTime;

typedef struct
{
	uint16_t push_time_counter;
	uint16_t release_time_counter;
	ButtonOperationMode CurrentOM;
	sParamAboutTime const *  p_spat;
}sParamSingleButton;

typedef struct
{
	uint8_t button_number;
	uint8_t ModuleLoopTimeInMS;
	X_Void (*init)(X_Void);
	CombineButtonValue (*get_value)(X_Void);
	X_Void (*config)(sParamSingleButton * p_spsb);
	X_Void (*click)(CombineButtonValue value);
	X_Void (*continus_click)(CombineButtonValue value);
	X_Void (*double_click)(CombineButtonValue value);
	X_Void (*long_push)(CombineButtonValue value);
	X_Void (*long_push_release)(CombineButtonValue value,uint16_t const*longpushtickcycle);

}sButtonModule;

typedef struct
{
	const sButtonModule  	*base;
	const uint8_t          	button_number;
	const StateSimpleParam  *p_monitor[];
}sButtonModuleExtern;



#define CUSTOM_BUTTON_MODULE_DEF(	p_button_module						\
									,button_number                      \
									,loop_time_in_ms					\
									,function_init						\
									,function_get_value					\
									,function_config					\
									,function_click_cb					\
									,function_continus_click_cb			\
									,function_double_click_cb			\
									,fucntion_long_push_cb				\
									,function_long_push_release_cb)		\
static const sButtonModule CONCAT_2(p_button_module, _entry) = {		\
		button_number,													\
		loop_time_in_ms,												\
		function_init,													\
		function_get_value,												\
		function_config,												\
		function_click_cb,												\
		function_continus_click_cb,										\
		function_double_click_cb,										\
		fucntion_long_push_cb,											\
		function_long_push_release_cb,									\
};


X_Void ButtonStateMonitor(const sButtonModuleExtern *p_sbm,CombineButtonValue *value);
X_Void SetCurrentButtonConfigMode(CombineButtonValue mode);


#endif
