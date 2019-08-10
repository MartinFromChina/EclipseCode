#ifndef __BUTTONSTATEMONITOR_H
#define __BUTTONSTATEMONITOR_H

#include "..\..\CommonSource\KeilMDK.h"
#include "..\..\CommonSource\StateMachine\StateMachine.h"
#include "ButtonModule.h"

#define MAX_BUTTON_MONITOR_HOP_TIMES_IN_ONE_CALL   2

StateNumber CustomizedBM_InitAction(StateNumber current_state);
StateNumber CustomizedBM_StartAction(StateNumber current_state);
StateNumber CustomizedBM_ClickDetectAction(StateNumber current_state);
StateNumber CustomizedBM_LongPushDetectAction(StateNumber current_state);
StateNumber CustomizedBM_SureLongPushAction(StateNumber current_state);
StateNumber CustomizedBM_LongPushReleaseDetectAction(StateNumber current_state);
StateNumber CustomizedBM_SureLongPushReleaseAction(StateNumber current_state);
StateNumber CustomizedBM_ClickReleaseDetectAction(StateNumber current_state);
StateNumber CustomizedBM_DoubleClickDetectAction(StateNumber current_state);
StateNumber CustomizedBM_SureClickAction(StateNumber current_state);
StateNumber CustomizedBM_SureDoubleClickAction(StateNumber current_state);

/*************************************************1**************************************************/
#define CUSTOM_BUTTON_MONITOR_1_DEF(p_button_module						\
									,loop_time_in_ms						\
									,function_init						\
									,function_get_value					\
									,function_config					\
									,function_click_cb					\
									,function_double_click_cb			\
									,fucntion_long_push_cb				\
									,function_long_push_release_cb		\
)																		\
		CUSTOM_BUTTON_MODULE_DEF(p_button_module						\
								,1										\
								,loop_time_in_ms						\
								,function_init							\
								,function_get_value						\
								,function_config						\
								,function_click_cb						\
								,function_double_click_cb				\
								,fucntion_long_push_cb					\
								,function_long_push_release_cb			\
);																		\
static const StateAction CONCAT_2(p_button_module, _ButtonStateAction)[] = {	\
		{CustomizedBM_InitAction},												\
		{CustomizedBM_StartAction},												\
		{CustomizedBM_ClickDetectAction},										\
		{CustomizedBM_LongPushDetectAction},									\
		{CustomizedBM_SureLongPushAction},										\
		{CustomizedBM_LongPushReleaseDetectAction},								\
		{CustomizedBM_SureLongPushReleaseAction},								\
		{CustomizedBM_ClickReleaseDetectAction},								\
		{CustomizedBM_DoubleClickDetectAction},									\
		{CustomizedBM_SureClickAction},											\
		{CustomizedBM_SureDoubleClickAction},									\
};																				\
																				\
/***********0************/														\
APP_SIMPLE_STATE_MACHINE_DEF_WITH_OUT_POINTER(p_button_module_motion_action_0																			\
									,sizeof(CONCAT_2(p_button_module, _ButtonStateAction))/sizeof(CONCAT_2(p_button_module, _ButtonStateAction)[0])   	\
									,MAX_BUTTON_MONITOR_HOP_TIMES_IN_ONE_CALL																			\
									,&CONCAT_2(p_button_module, _ButtonStateAction)[0]);																\
/***********end************/														\
static const sButtonModuleExtern CONCAT_2(p_button_module, _with_monitor) = {		\
		&CONCAT_2(p_button_module, _entry),											\
		1,																			\
		{																			\
		&CONCAT_2(p_button_module_motion_action_0, _entry),							\
		},																			\
};																					\
																					\
static const sButtonModuleExtern * p_button_module = &CONCAT_2(p_button_module, _with_monitor)

/*************************************************2**************************************************/
#define CUSTOM_BUTTON_MONITOR_2_DEF(p_button_module						\
									,loop_time_in_ms					\
									,function_init						\
									,function_get_value					\
									,function_config					\
									,function_click_cb					\
									,function_double_click_cb			\
									,fucntion_long_push_cb				\
									,function_long_push_release_cb		\
									,function_state_recorder				\
)																		\
		CUSTOM_BUTTON_MODULE_DEF(p_button_module						\
								,2										\
								,loop_time_in_ms						\
								,function_init							\
								,function_get_value						\
								,function_config						\
								,function_click_cb						\
								,function_double_click_cb				\
								,fucntion_long_push_cb					\
								,function_long_push_release_cb			\
								,function_state_recorder					\
);																		\
static const StateAction CONCAT_2(p_button_module, _ButtonStateAction)[] = {	\
		{CustomizedBM_InitAction},												\
		{CustomizedBM_StartAction},												\
		{CustomizedBM_ClickDetectAction},										\
		{CustomizedBM_LongPushDetectAction},									\
		{CustomizedBM_SureLongPushAction},										\
		{CustomizedBM_LongPushReleaseDetectAction},								\
		{CustomizedBM_SureLongPushReleaseAction},								\
		{CustomizedBM_ClickReleaseDetectAction},								\
		{CustomizedBM_DoubleClickDetectAction},									\
		{CustomizedBM_SureClickAction},											\
		{CustomizedBM_SureDoubleClickAction},									\
};																				\
																				\
/***********0************/															\
APP_SIMPLE_STATE_MACHINE_DEF_WITH_OUT_POINTER(p_button_module_motion_action_0																			\
									,sizeof(CONCAT_2(p_button_module, _ButtonStateAction))/sizeof(CONCAT_2(p_button_module, _ButtonStateAction)[0])   	\
									,MAX_BUTTON_MONITOR_HOP_TIMES_IN_ONE_CALL																			\
									,&CONCAT_2(p_button_module, _ButtonStateAction)[0]);																\
/***********1************/															\
APP_SIMPLE_STATE_MACHINE_DEF_WITH_OUT_POINTER(p_button_module_motion_action_1																			\
							,sizeof(CONCAT_2(p_button_module, _ButtonStateAction))/sizeof(CONCAT_2(p_button_module, _ButtonStateAction)[0])   			\
							,MAX_BUTTON_MONITOR_HOP_TIMES_IN_ONE_CALL																					\
							,&CONCAT_2(p_button_module, _ButtonStateAction)[0]);																		\
/***********end************/														\
static const sButtonModuleExtern CONCAT_2(p_button_module, _with_monitor) = {		\
		&CONCAT_2(p_button_module, _entry),											\
		2,																			\
		{																			\
		&CONCAT_2(p_button_module_motion_action_0, _entry),							\
		&CONCAT_2(p_button_module_motion_action_1, _entry),							\
		},																			\
};																					\
																					\
static const sButtonModuleExtern * p_button_module = &CONCAT_2(p_button_module, _with_monitor)

/*************************************************3**************************************************/
#define CUSTOM_BUTTON_MONITOR_3_DEF(p_button_module						\
									,loop_time_in_ms						\
									,function_init						\
									,function_get_value					\
									,function_config					\
									,function_click_cb					\
									,function_double_click_cb			\
									,fucntion_long_push_cb				\
									,function_long_push_release_cb		\
									,function_state_recorder					\
)																		\
		CUSTOM_BUTTON_MODULE_DEF(p_button_module						\
								,3										\
								,loop_time_in_ms						\
								,function_init							\
								,function_get_value						\
								,function_config						\
								,function_click_cb						\
								,function_double_click_cb				\
								,fucntion_long_push_cb					\
								,function_long_push_release_cb			\
								,function_state_recorder					\
);																		\
static const StateAction CONCAT_2(p_button_module, _ButtonStateAction)[] = {	\
		{CustomizedBM_InitAction},												\
		{CustomizedBM_StartAction},												\
		{CustomizedBM_ClickDetectAction},										\
		{CustomizedBM_LongPushDetectAction},									\
		{CustomizedBM_SureLongPushAction},										\
		{CustomizedBM_LongPushReleaseDetectAction},								\
		{CustomizedBM_SureLongPushReleaseAction},								\
		{CustomizedBM_ClickReleaseDetectAction},								\
		{CustomizedBM_DoubleClickDetectAction},									\
		{CustomizedBM_SureClickAction},											\
		{CustomizedBM_SureDoubleClickAction},									\
};																				\
																				\
/***********0************/															\
APP_SIMPLE_STATE_MACHINE_DEF_WITH_OUT_POINTER(p_button_module_motion_action_0																			\
									,sizeof(CONCAT_2(p_button_module, _ButtonStateAction))/sizeof(CONCAT_2(p_button_module, _ButtonStateAction)[0])   	\
									,MAX_BUTTON_MONITOR_HOP_TIMES_IN_ONE_CALL																			\
									,&CONCAT_2(p_button_module, _ButtonStateAction)[0]);																\
/***********1************/															\
APP_SIMPLE_STATE_MACHINE_DEF_WITH_OUT_POINTER(p_button_module_motion_action_1																			\
							,sizeof(CONCAT_2(p_button_module, _ButtonStateAction))/sizeof(CONCAT_2(p_button_module, _ButtonStateAction)[0])   			\
							,MAX_BUTTON_MONITOR_HOP_TIMES_IN_ONE_CALL																					\
							,&CONCAT_2(p_button_module, _ButtonStateAction)[0]);																		\
/***********2************/															\
APP_SIMPLE_STATE_MACHINE_DEF_WITH_OUT_POINTER(p_button_module_motion_action_2																			\
							,sizeof(CONCAT_2(p_button_module, _ButtonStateAction))/sizeof(CONCAT_2(p_button_module, _ButtonStateAction)[0])   			\
							,MAX_BUTTON_MONITOR_HOP_TIMES_IN_ONE_CALL																					\
							,&CONCAT_2(p_button_module, _ButtonStateAction)[0]);																		\
/***********end************/														\
static const sButtonModuleExtern CONCAT_2(p_button_module, _with_monitor) = {		\
		&CONCAT_2(p_button_module, _entry),											\
		3,																			\
		{																			\
		&CONCAT_2(p_button_module_motion_action_0, _entry),							\
		&CONCAT_2(p_button_module_motion_action_1, _entry),							\
		&CONCAT_2(p_button_module_motion_action_2, _entry),							\
		},																			\
};																					\
																					\
static const sButtonModuleExtern * p_button_module = &CONCAT_2(p_button_module, _with_monitor)



/*************************************************8**************************************************/
#define CUSTOM_BUTTON_MONITOR_8_DEF(p_button_module						\
									,loop_time_in_ms						\
									,function_init						\
									,function_get_value					\
									,function_config					\
									,function_click_cb					\
									,function_double_click_cb			\
									,fucntion_long_push_cb				\
									,function_long_push_release_cb		\
									,function_state_recorder				\
)																		\
		CUSTOM_BUTTON_MODULE_DEF(p_button_module						\
								,8										\
								,loop_time_in_ms						\
								,function_init							\
								,function_get_value						\
								,function_config						\
								,function_click_cb						\
								,function_double_click_cb				\
								,fucntion_long_push_cb					\
								,function_long_push_release_cb			\
								,function_state_recorder				\
);																		\
static const StateAction CONCAT_2(p_button_module, _ButtonStateAction)[] = {	\
		{CustomizedBM_InitAction},												\
		{CustomizedBM_StartAction},												\
		{CustomizedBM_ClickDetectAction},										\
		{CustomizedBM_LongPushDetectAction},									\
		{CustomizedBM_SureLongPushAction},										\
		{CustomizedBM_LongPushReleaseDetectAction},								\
		{CustomizedBM_SureLongPushReleaseAction},								\
		{CustomizedBM_ClickReleaseDetectAction},								\
		{CustomizedBM_DoubleClickDetectAction},									\
		{CustomizedBM_SureClickAction},											\
		{CustomizedBM_SureDoubleClickAction},									\
};																				\
																				\
/***********0************/															\
APP_SIMPLE_STATE_MACHINE_DEF_WITH_OUT_POINTER(p_button_module_motion_action_0																			\
									,sizeof(CONCAT_2(p_button_module, _ButtonStateAction))/sizeof(CONCAT_2(p_button_module, _ButtonStateAction)[0])   	\
									,MAX_BUTTON_MONITOR_HOP_TIMES_IN_ONE_CALL																			\
									,&CONCAT_2(p_button_module, _ButtonStateAction)[0]);																\
/***********1************/															\
APP_SIMPLE_STATE_MACHINE_DEF_WITH_OUT_POINTER(p_button_module_motion_action_1																			\
							,sizeof(CONCAT_2(p_button_module, _ButtonStateAction))/sizeof(CONCAT_2(p_button_module, _ButtonStateAction)[0])   			\
							,MAX_BUTTON_MONITOR_HOP_TIMES_IN_ONE_CALL																					\
							,&CONCAT_2(p_button_module, _ButtonStateAction)[0]);																		\
/***********2************/															\
APP_SIMPLE_STATE_MACHINE_DEF_WITH_OUT_POINTER(p_button_module_motion_action_2																			\
							,sizeof(CONCAT_2(p_button_module, _ButtonStateAction))/sizeof(CONCAT_2(p_button_module, _ButtonStateAction)[0])   			\
							,MAX_BUTTON_MONITOR_HOP_TIMES_IN_ONE_CALL																					\
							,&CONCAT_2(p_button_module, _ButtonStateAction)[0]);																		\
/***********3************/															\
APP_SIMPLE_STATE_MACHINE_DEF_WITH_OUT_POINTER(p_button_module_motion_action_3																			\
							,sizeof(CONCAT_2(p_button_module, _ButtonStateAction))/sizeof(CONCAT_2(p_button_module, _ButtonStateAction)[0])   			\
							,MAX_BUTTON_MONITOR_HOP_TIMES_IN_ONE_CALL																					\
							,&CONCAT_2(p_button_module, _ButtonStateAction)[0]);																		\
/***********4************/															\
APP_SIMPLE_STATE_MACHINE_DEF_WITH_OUT_POINTER(p_button_module_motion_action_4																			\
							,sizeof(CONCAT_2(p_button_module, _ButtonStateAction))/sizeof(CONCAT_2(p_button_module, _ButtonStateAction)[0])   			\
							,MAX_BUTTON_MONITOR_HOP_TIMES_IN_ONE_CALL																					\
							,&CONCAT_2(p_button_module, _ButtonStateAction)[0]);																		\
/***********5************/															\
APP_SIMPLE_STATE_MACHINE_DEF_WITH_OUT_POINTER(p_button_module_motion_action_5																			\
							,sizeof(CONCAT_2(p_button_module, _ButtonStateAction))/sizeof(CONCAT_2(p_button_module, _ButtonStateAction)[0])   			\
							,MAX_BUTTON_MONITOR_HOP_TIMES_IN_ONE_CALL																					\
							,&CONCAT_2(p_button_module, _ButtonStateAction)[0]);																		\
/***********6************/															\
APP_SIMPLE_STATE_MACHINE_DEF_WITH_OUT_POINTER(p_button_module_motion_action_6																			\
							,sizeof(CONCAT_2(p_button_module, _ButtonStateAction))/sizeof(CONCAT_2(p_button_module, _ButtonStateAction)[0])   			\
							,MAX_BUTTON_MONITOR_HOP_TIMES_IN_ONE_CALL																					\
							,&CONCAT_2(p_button_module, _ButtonStateAction)[0]);																		\
/***********7************/															\
APP_SIMPLE_STATE_MACHINE_DEF_WITH_OUT_POINTER(p_button_module_motion_action_7																			\
							,sizeof(CONCAT_2(p_button_module, _ButtonStateAction))/sizeof(CONCAT_2(p_button_module, _ButtonStateAction)[0])   			\
							,MAX_BUTTON_MONITOR_HOP_TIMES_IN_ONE_CALL																					\
							,&CONCAT_2(p_button_module, _ButtonStateAction)[0]);																		\
/***********end************/														\
static const sButtonModuleExtern CONCAT_2(p_button_module, _with_monitor) = {		\
		&CONCAT_2(p_button_module, _entry),											\
		8,																			\
		{																			\
		&CONCAT_2(p_button_module_motion_action_0, _entry),							\
		&CONCAT_2(p_button_module_motion_action_1, _entry),							\
		&CONCAT_2(p_button_module_motion_action_2, _entry),							\
		&CONCAT_2(p_button_module_motion_action_3, _entry),							\
		&CONCAT_2(p_button_module_motion_action_4, _entry),							\
		&CONCAT_2(p_button_module_motion_action_5, _entry),							\
		&CONCAT_2(p_button_module_motion_action_6, _entry),							\
		&CONCAT_2(p_button_module_motion_action_7, _entry),							\
		},																			\
};																					\
																					\
static const sButtonModuleExtern * p_button_module = &CONCAT_2(p_button_module, _with_monitor)


#endif
