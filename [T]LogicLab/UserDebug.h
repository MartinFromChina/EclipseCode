#ifndef __USERDEBUG_H
#define __USERDEBUG_H

#include "..\CommonSource\CharStringDebug\CharStringDebugModule.h"
#include "..\CommonSource\AppError.h"

/*********************
RTT_DEBUG_ONCE_ENTRY_DEF(p_entry_name,init_value)
RTT_DEBUG_COUNTER_ENTRY_DEF(p_entry_name,init_value)    

SEGGER_RTT_Debug(flag,message)   
SEGGER_RTT_Debug_Once(flag,entry,once_ref,message)   
SEGGER_RTT_Debug_Counter(flag,entry,counter_ref,message)   
	**********************/
#define RTT_DEBUG_ONCE_ENTRY_DEF(p_entry_name,init_value)      		STRING_DEBUG_ONCE_ENTRY_DEF(p_entry_name,init_value)
#define RTT_DEBUG_COUNTER_ENTRY_DEF(p_entry_name,init_value)   		STRING_DEBUG_COUNTER_ENTRY_DEF(p_entry_name,init_value)

#define SEGGER_RTT_Debug(flag,message)   							String_Debug(flag,message)
#define SEGGER_RTT_Debug_Once(flag,entry,once_ref,message)   		String_Debug_Once(flag,entry,once_ref,message)
#define SEGGER_RTT_Debug_Counter(flag,entry,counter_ref,message)  	String_Debug_Counter(flag,entry,counter_ref,message)

#include "TB_TinyDancer\DongleTask\DongleTaskDebugSwitch.h"
	
#endif

