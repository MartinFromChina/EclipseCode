#ifndef __STRINGDEBUGMODULECONF_H
#define __STRINGDEBUGMODULECONF_H

//#include "user_log.h"
#include <stdio.h>

#define USE_ARM_GCC              0
#define USE_STRING_DEBUG         1
#define USE_ERROR_TRANSLATE      1
#define STRING_DEBUG_BUF_NUMBER  10
#define MAX_STRING_LENGTH        50
#define MAX_STRING_ARG_NUMBER    7

#define DEBUG_PUSH_METHOD			   DebugPushString

#define DEBUG_POP_METHOD       		   printf
#define DEBUG_ENTER_CRITICAL_METHOD
#define DEBUG_EXIT_CRITICAL_METHOD


//#include "..\test_target_usart\test_target_usart_printf_switch.h"
#define FOW_NOW_DEBUG			1
#define COUNTR_DEBUG            1
#define ENTRY_DEBUG             1


#endif
