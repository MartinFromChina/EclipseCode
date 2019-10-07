#ifndef __M_FLASH_HANDLER_CONFIG_DEFAULT_H
#define __M_FLASH_HANDLER_CONFIG_DEFAULT_H

/************************************************************************************/
#define FLASH_POINTER_TYPE            		uint32_t// void or uint32_t
#define FLASH_RETURN_TYPE            		uint32_t// void or uint32_t or int32
typedef FLASH_RETURN_TYPE (*onMyFlashRead)(uint32_t addr,FLASH_POINTER_TYPE *p_dest,uint32_t length_in_bytes);
typedef FLASH_RETURN_TYPE (*onMyFlashWrite)(uint32_t addr,FLASH_POINTER_TYPE const*p_src,uint32_t length_in_bytes);
typedef FLASH_RETURN_TYPE (*onMyFlashErase)(uint32_t addr,uint32_t length_in_bytes);

/************************************************************************************/
#define M_FLASH_MAX_SIMPLE_REQUEST_HOLD_COUNT   20
#define M_FLASH_MAX_REQUEST_HOLD_COUNT   		30
#define M_FLASH_READ_IMMEDIATELY                0
#define M_FLASH_ENABLE_PARAM_CHECK				1
#define M_FLASH_ENABLE_USER_MULTI_PARTITION     1
#define M_FLASH_TIMER_MODULE_INTERVAL_IN_MS 		   8
#define M_FLASH_WAIT_OPERATION_FINISHED_TIME_IN_MS     30

#define M_FLASH_ENTER_CRITICAL_REGION_METHOD
#define M_FLASH_EXIT_CRITICAL_REGION_METHOD

/************************************************************************************/
#define USE_MY_FLASH_EVENT_HANDLER_DEBUG		1
#define MAX_MY_FLASH_EVENT_DEBUG_ID_COUNT		5
#endif
