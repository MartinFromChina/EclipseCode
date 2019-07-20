#ifndef __CHARSTRINGMODULECONFIG_H
#define __CHARSTRINGMODULECONFIG_H

#include <stdio.h>
#include "..\IrqAndTimer\app_timer.h"
#include "..\LogFile\user_log.h"

#define USE_ARM_GCC              0
#define USE_STRING_DEBUG         1
#define USE_ERROR_TRANSLATE      1

#define USE_OPERATION_SYSTEM     0
#define MAX_STRING_LENGTH        50
#define MAX_STRING_ARG_NUMBER    7
#define USER_MAX_STRING_LENGTH   (MAX_STRING_LENGTH-MAX_STRING_ARG_NUMBER)
#define STRING_DEBUG_BUF_NUMBER  20

#define GET_DEBUG_SEMPHORE_METHOD
#define RELEASE_DEBUG_SEMPHORE_METHOD
#define DEBUG_ENTER_CRITICAL_METHOD   AppTimerLocked()
#define DEBUG_EXIT_CRITICAL_METHOD    AppTimerUnlocked()

#define DEBUG_PUSH_METHOD			   PushStringIntoCharBuf
#define DEBUG_POP_METHOD       		   Log//printf

#define FOW_NOW_DEBUG			1
#define COUNTR_DEBUG            0
#define ENTRY_DEBUG             1

#endif
