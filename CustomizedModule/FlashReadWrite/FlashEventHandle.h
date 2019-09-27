#ifndef __FLASHEVENTHANDLE_H
#define __FLASHEVENTHANDLE_H

#include "..\..\CommonSource\KeilMDK.h"
#include "..\..\CommonSource\AppError.h"
#include "..\..\CommonSource\MulLoopQueue\loop_queues.h"
#include "..\..\CommonSource\MulLoopQueue\m_priority_queues.h"

#ifdef USE_LOCAL_FLASH_HANDLER_CONFIG
	#include "m_flash_handler_config_local.h"
#else
	#include "m_flash_handler_config_default.h"
#endif

typedef enum
{
	FO_Init = 0,
	FO_UnInit,
	FO_Read,
	FO_Write,
	FO_Erase,
	FO_SectorRead,
	FO_SectorWrite,
}eFlashDebugOperation;

#if (M_FLASH_MULTI_PARTITION == 1)
typedef struct
{
	uint16_t PhysicalDiskID; // the number ID of mul flash region which is the min erase region ,usually 4096bytes as a page in flash;
	uint16_t VirtualSectorNumber; //the number ID of sectors which in a min erase region;
}sMyFlashParition;
#endif

#if (M_FLASH_ENABLE_BOUNDARY_CHECK == 1)

#endif

typedef struct
{
	X_Boolean isInitOK;
}sMyFlashModuleManager;

typedef struct   _sMyFlashEventHandler    sMyFlashEventHandler;
struct _sMyFlashEventHandler
{
	sMyFlashModuleManager  *p_manager;
	sListManager		   const* p_loop_queue;
	sMyPriorityListManager const* p_priority_queue;
	X_Void 						(*onDebug)(eFlashDebugOperation e_fdo,uint8_t operation_ID,const sMyFlashEventHandler * p_handler);
	uint32_t                    (*onDebugParamCollect)(eSimpleQueueOperation op,uint32_t param);
};

typedef   X_Void (*m_flash_event_debug_handler)(const sMySingleLinkList * s_sll);

typedef struct
{
	eFlashDebugOperation eFDO;
	m_flash_event_debug_handler debug_handler[MAX_MY_FLASH_EVENT_DEBUG_ID_COUNT];
}sMyFlashEventMessageDebugTable;

#define APP_FLASH_EVENT_HANDLER_DEFINE(p_handler											\
										,flash_base_addr,flash_total_size_in_bytes			\
										,flash_erase_unit,flash_program_unit 				\
										,flash_user_define_sector_size						\
										,flash_user_define_total_sector_count				\
										,init,uninit,read,write,erase,does_busy				\
										,debug_method,param_debug							\
										)													\
	SIMPLE_LOOPQUEUE_DEF_WITHOUT_POINTER(p_simple_queue_for_flash,(flash_user_define_total_sector_count + 5));										 \
	APP_PRIORITY_QUEUE_DEF_WITH_LIMITATION_WITHOUT_POINTER(p_prio_queue_for_flash,flash_user_define_total_sector_count,X_True,X_Null,X_Null,X_Null); \
	static sMyFlashModuleManager      CONCAT_2(p_handler,_flash_manager_rntry) = {X_False}; \
	static const sMyFlashEventHandler CONCAT_2(p_handler,_flash_handle_entry) = {			\
	&CONCAT_2(p_handler,_flash_manager_rntry)												\
	,&CONCAT_2(p_simple_queue_for_flash,_loopqueue_entry)									\
	,&CONCAT_2(p_prio_queue_for_flash,_queue_entry)											\
	,debug_method																			\
	,param_debug																			\
	};																						\
	static sMyFlashEventHandler const* p_handler = &CONCAT_2(p_handler,_flash_handle_entry)

typedef   X_Void (*onReadFinished)(X_Void);
typedef   X_Void (*onWriteFinished)(X_Void);
typedef   X_Void (*onEraseFinished)(X_Void);

m_app_result mFlashEventInit(const sMyFlashEventHandler *p_handler);
m_app_result mFlashEventUnInit(const sMyFlashEventHandler *p_handler);
X_Void 		 mFlashEventHandlerRun(const sMyFlashEventHandler *p_handler);

m_app_result mFlashReadRequest(const sMyFlashEventHandler *p_handler,uint32_t read_start_addr,uint32_t read_length,void const * p_dest,onReadFinished read_cb);
m_app_result mFlashWriteRequest(const sMyFlashEventHandler *p_handler,uint32_t write_start_addr,uint32_t write_length,void const * p_src,onWriteFinished write_cb);
m_app_result mFlashEraseRequest(const sMyFlashEventHandler *p_handler,uint32_t Erase_start_addr,uint32_t erase_length,void const * p_src,onEraseFinished erase_cb);

#if (M_FLASH_MULTI_PARTITION == 1)
m_app_result mFlashSectorReadRequest(const sMyFlashEventHandler *p_handler,uint32_t sector_number,uint32_t length,void const * p_dest,onReadFinished read_cb);
m_app_result mFlashSectorWriteRequest(const sMyFlashEventHandler *p_handler,uint32_t sector_number,uint32_t length,void const * p_src,onWriteFinished write_cb);

m_app_result mFlashEventHandlerHold(const sMyFlashEventHandler *p_handler);
m_app_result mFlashEventHandlerBeginProcessing(const sMyFlashEventHandler *p_handler);
#endif

#endif
