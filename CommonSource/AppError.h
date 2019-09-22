#ifndef __APPERROR_H
#define __APPERROR_H

#include "KeilMDK.h"

typedef uint8_t						m_app_result;

#define APP_SUCCESSED       		0
#define APP_ERROR           		1
#define APP_BEYOND_SCOPE    		2
#define APP_POINTER_NULL    		3
#define APP_ALREADY_DONE_BEFORE 	4
#define APP_PARAM_ERROR				5

char *AppErrorGet(uint32_t error,char* p_cannot);

#endif
