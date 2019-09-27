#ifndef __M_FLASH_HANDLER_CONFIG_DEFAULT_H
#define __M_FLASH_HANDLER_CONFIG_DEFAULT_H

#define FLASH_POINTER_TYPE            		uint32_t// void or uint32_t
#define FLASH_RETURN_TYPE            		uint32_t// void or uint32_t or int32
typedef   FLASH_RETURN_TYPE (*onMyFlashRead)(uint32_t addr,FLASH_POINTER_TYPE *p_dest,uint32_t length_in_bytes);
typedef   FLASH_RETURN_TYPE (*onMyFlashWrite)(uint32_t addr,FLASH_POINTER_TYPE const*p_src,uint32_t length_in_bytes);
typedef   FLASH_RETURN_TYPE (*onMyFlashErase)(uint32_t addr,uint32_t length_in_bytes);

//#define M_FLASH_BASIC_UNIT_IN_BYTES             4096
#define M_FLASH_ENABLE_BOUNDARY_CHECK			1
#define M_FLASH_MULTI_PARTITION                 0

#define USE_MY_FLASH_EVENT_HANDLER_DEBUG		1
#define MAX_MY_FLASH_EVENT_DEBUG_ID_COUNT		5
#endif
