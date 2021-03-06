#ifndef __FLASHEVENTHANDLE_H
#define __FLASHEVENTHANDLE_H

#include "..\..\CommonSource\KeilMDK.h"
#include "..\..\CommonSource\AppError.h"
#include "..\..\CommonSource\MulLoopQueue\loop_queues.h"
#include "..\..\CommonSource\MulLoopQueue\m_priority_queues.h"
#include "..\..\CommonSource\StateMachine\StateMachine.h"

#ifdef USE_LOCAL_FLASH_HANDLER_CONFIG
	#include "m_flash_handler_config_local.h"
#else
	#include "m_flash_handler_config_default.h"
#endif

typedef enum
{
	MFO_Read,
	MFO_Write,
	MFO_Erase,
}eMyFlashOperation;

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

#if (M_FLASH_ENABLE_USER_MULTI_PARTITION == 1)
	typedef struct
	{
	#if (USE_MY_FLASH_EVENT_HANDLER_DEBUG == 1)
		const char * p_name;
	#endif
		const uint16_t sector_number;
		const uint16_t infor_data_length;// in bytes
		uint8_t const *p_buf;
	}sUserInforInFlashTable;

	typedef struct
	{
		uint16_t const EraseUnitNumber;// which user information belong to
		uint16_t const total_infor_count;
		sUserInforInFlashTable const *p_table;
	}sUserInformationInFlash;

	typedef struct
	{
		uint16_t UserInforCountbyErasePage;
		sUserInformationInFlash const* p_infor;
	}sUserInforPageTable;
#endif

typedef struct
{
	uint32_t read_start_addr;
	uint32_t read_length;
	FLASH_POINTER_TYPE  * p_dest;
	onMyFlashRead read_cb;
}sMyFlashReadParam;

typedef struct
{
	uint32_t write_start_addr;
	uint32_t write_length;
	FLASH_POINTER_TYPE const * p_src;
	onMyFlashWrite write_cb;
}sMyFlashWriteParam;

typedef struct
{
	uint32_t erase_start_addr;
	uint32_t erase_length;
	onMyFlashErase erase_cb;
}sMyFlashEraseParam;

typedef struct
{
	eMyFlashOperation opcode;
	union {
			sMyFlashReadParam  fr;
			sMyFlashWriteParam fw;
			sMyFlashEraseParam fe;
		};
}sMyFlashOperationParam;

typedef struct
{
	X_Boolean isInitOK;
#if (M_FLASH_ENABLE_USER_MULTI_PARTITION == 1)
	uint16_t  sector_count_in_erase_unit;
	sMyFlashWriteParam simul_write_param[M_FLASH_MAX_REQUEST_HOLD_COUNT];
#endif
	sMyFlashOperationParam fop[M_FLASH_MAX_SIMPLE_REQUEST_HOLD_COUNT];
}sMyFlashModuleManager;

typedef struct
{
	uint32_t base_addr;
	uint32_t total_size_in_bytes;
	uint32_t erase_uint;
	uint32_t program_unit;
}sMyFlashEventBasicParam;

typedef struct
{
	uint32_t sector_size;
	uint32_t total_sector_count;
}sMyFlashEventUserParam;

typedef struct
{
	X_Void				(*init)(X_Void);
	X_Void				(*uninit)(X_Void);
	onMyFlashRead      	read_evt_handler;
	onMyFlashWrite		write_evt_handler;
	onMyFlashErase		erase_evt_handler;
	X_Boolean			(*doesbusy)(X_Void);
	X_Void				(*busy_flag_set)(X_Boolean isBusy);
}sMyFlashEventAction;


typedef struct
{
	s_StateMachineParam 	base;
	X_Boolean               isStateMachineStop;
	StateNumber 			LatestStateBeforeRequestCheck;
	uint16_t 				wait_counter;
}sMyFlashStateParamExtern;
typedef struct
{
  uint16_t (*get_ms)(X_Void);
  X_Void   (*set_ms)(uint16_t time);
}sMyFlashEventTimer;
typedef struct
{
	sMyFlashStateParamExtern *p_fspe;
	StateSimpleParam  		const*p_flash_state;
}sMyFlashEventHandlerState;

typedef struct   _sMyFlashEventHandler    sMyFlashEventHandler;
struct _sMyFlashEventHandler
{
	sMyFlashModuleManager   	 * p_manager;
	sMyFlashEventBasicParam const* p_basic_param;
	sMyFlashEventUserParam  const* p_user_param;
	sListManager		    const* p_loop_queue;
#if (M_FLASH_ENABLE_USER_MULTI_PARTITION == 1)
	sUserInforPageTable     	 *p_user_infor_table;
	sMyPriorityListManager  const* p_priority_queue;
	X_Void						(*onUserInforConfig)(sUserInforPageTable *p_user_infor_table);
#endif
	sMyFlashEventAction     const*p_action;
	sMyFlashEventTimer		const*p_timer;
	sMyFlashEventHandlerState const*p_state_machine;
	X_Void 						(*onDebug)(eFlashDebugOperation e_fdo,uint8_t operation_ID,const sMyFlashEventHandler * p_handler);
	uint32_t                    (*onDebugParamCollect)(eSimpleQueueOperation op,uint32_t param);
};

typedef   X_Void (*m_flash_event_debug_handler)(const sMyFlashEventHandler * p_handler);

typedef struct
{
	eFlashDebugOperation eFDO;
	m_flash_event_debug_handler debug_handler[MAX_MY_FLASH_EVENT_DEBUG_ID_COUNT];
}sMyFlashEventMessageDebugTable;


#if (M_FLASH_ENABLE_USER_MULTI_PARTITION == 1)
	#define APP_FLASH_EVENT_HANDLER_DEFINE(p_handler											\
										,flash_base_addr,flash_total_size_in_bytes			\
										,flash_erase_unit,flash_program_unit 				\
										,flash_user_define_sector_size						\
										,flash_user_define_total_sector_count				\
										,init,uninit,read,write,erase,does_busy,busy_flag_set \
										,user_infor_config									\
										,time_set_ms,time_get_ms							\
										,debug_method,param_debug							\
										)													\
	SIMPLE_LOOPQUEUE_DEF_WITHOUT_POINTER(p_simple_queue_for_flash,M_FLASH_MAX_SIMPLE_REQUEST_HOLD_COUNT);										 \
	APP_PRIORITY_QUEUE_DEF_WITHOUT_POINTER(p_prio_queue_for_flash,M_FLASH_MAX_REQUEST_HOLD_COUNT,(flash_user_define_total_sector_count-1),X_True); \
	static 		 sUserInforPageTable       CONCAT_2(p_handler,_user_infor_page_table_entry) = {0,X_Null};	\
	static 		 sMyFlashModuleManager     CONCAT_2(p_handler,_flash_manager_entry) = {X_False,0,{}}; 		\
	static const sMyFlashEventTimer        CONCAT_2(p_handler,_flash_timer_entry)  = {time_set_ms,time_get_ms};\
	static const sMyFlashEventBasicParam   CONCAT_2(p_handler,_flash_basicparam) = {flash_base_addr,flash_total_size_in_bytes,flash_erase_unit,flash_program_unit};\
	static const sMyFlashEventUserParam    CONCAT_2(p_handler,_flash_userparam) = {flash_user_define_sector_size,flash_user_define_total_sector_count};      \
	static const sMyFlashEventAction       CONCAT_2(p_handler,_flash_action) = {init,uninit,read,write,erase,does_busy,busy_flag_set};	\
	static const StateAction CONCAT_2(p_flash_event, _FlashAction)[] = {			\
					{X_Null},												\
					{X_Null},												\
					{X_Null},										\
					{X_Null},									\
					{X_Null},										\
					{X_Null},								\
					{X_Null},								\
					{X_Null},								\
					{X_Null},									\
					{X_Null},											\
					{X_Null},									\
			};																				\
																							\
			/***********************/														\
			APP_SIMPLE_STATE_MACHINE_DEF_WITH_OUT_POINTER(p_falsh_event_state_action		\
												,sizeof(CONCAT_2(p_flash_event, _FlashAction))/sizeof(CONCAT_2(p_flash_event, _FlashAction)[0])   	\
												,1																									\
												,&CONCAT_2(p_flash_event, _FlashAction)[0]);			\
			static sMyFlashStateParamExtern    CONCAT_2(p_handler,_s_MFSPE) = {{DEFAULT_STATE_NUMBER},X_False,DEFAULT_STATE_NUMBER,0};				\
			static const sMyFlashEventHandlerState CONCAT_2(p_handler,_s_MFEHS) = {			\
					&CONCAT_2(p_handler,_s_MFSPE)											\
					,&CONCAT_2(p_falsh_event_state_action,_entry)};							\
			/***********************/														\
	static const sMyFlashEventHandler CONCAT_2(p_handler,_flash_handle_entry) = {			\
	 &CONCAT_2(p_handler,_flash_manager_entry)												\
	,&CONCAT_2(p_handler,_flash_basicparam)													\
	,&CONCAT_2(p_handler,_flash_userparam)													\
	,&CONCAT_2(p_simple_queue_for_flash,_loopqueue_entry)									\
	,& CONCAT_2(p_handler,_user_infor_page_table_entry)										\
	,&CONCAT_2(p_prio_queue_for_flash,_priority_queue_entry)								\
	,user_infor_config																		\
	,&CONCAT_2(p_handler,_flash_action)														\
	,&CONCAT_2(p_handler,_flash_timer_entry)												\
	,&CONCAT_2(p_handler,_s_MFEHS)															\
	,debug_method																			\
	,param_debug																			\
	};																						\
	static sMyFlashEventHandler const* p_handler = &CONCAT_2(p_handler,_flash_handle_entry)
#else
	#define APP_FLASH_EVENT_HANDLER_DEFINE(p_handler											\
										,flash_base_addr,flash_total_size_in_bytes			\
										,flash_erase_unit,flash_program_unit 				\
										,flash_user_define_sector_size						\
										,flash_user_define_total_sector_count				\
										,init,uninit,read,write,erase,does_busy,busy_flag_set \
										,user_infor_config									\
										,time_set_ms,time_get_ms							\
										,debug_method,param_debug							\
										)													\
	SIMPLE_LOOPQUEUE_DEF_WITHOUT_POINTER(p_simple_queue_for_flash,M_FLASH_MAX_SIMPLE_REQUEST_HOLD_COUNT);										 \
	static 		 sMyFlashModuleManager      CONCAT_2(p_handler,_flash_manager_entry) = {X_False,{}}; 		\
	static const sMyFlashEventTimer         CONCAT_2(p_handler,_flash_timer_entry)  = {time_set_ms,time_get_ms};\
	static const sMyFlashEventBasicParam   CONCAT_2(p_handler,_flash_basicparam) = {flash_base_addr,flash_total_size_in_bytes,flash_erase_unit,flash_program_unit};\
	static const sMyFlashEventUserParam    CONCAT_2(p_handler,_flash_userparam) = {flash_user_define_sector_size,flash_user_define_total_sector_count};      \
	static const sMyFlashEventAction       CONCAT_2(p_handler,_flash_action) = {init,uninit,read,write,erase,does_busy,busy_flag_set};	\
	static const StateAction CONCAT_2(p_flash_event, _FlashAction)[] = {			\
					{X_Null},												\
					{X_Null},												\
					{X_Null},										\
					{X_Null},									\
					{X_Null},										\
					{X_Null},								\
					{X_Null},								\
					{X_Null},								\
					{X_Null},									\
					{X_Null},											\
					{X_Null},									\
			};																				\
																							\
			/***********************/														\
			APP_SIMPLE_STATE_MACHINE_DEF_WITH_OUT_POINTER(p_falsh_event_state_action		\
												,sizeof(CONCAT_2(p_flash_event, _FlashAction))/sizeof(CONCAT_2(p_flash_event, _FlashAction)[0])   	\
												,1																									\
												,&CONCAT_2(p_flash_event, _FlashAction)[0]);														\
			static sMyFlashStateParamExtern    CONCAT_2(p_handler,_s_MFSPE) = {{DEFAULT_STATE_NUMBER},X_False,DEFAULT_STATE_NUMBER,0};				\
			static const sMyFlashEventHandlerState CONCAT_2(p_handler,_s_MFEHS) = {		\
					&CONCAT_2(p_handler,_s_MFSPE)										\
					,&CONCAT_2(p_falsh_event_state_action,_entry)};						\
			/***********************/													\
	static const sMyFlashEventHandler CONCAT_2(p_handler,_flash_handle_entry) = {			\
	 &CONCAT_2(p_handler,_flash_manager_entry)												\
	,&CONCAT_2(p_handler,_flash_basicparam)													\
	,&CONCAT_2(p_handler,_flash_userparam)													\
	,&CONCAT_2(p_simple_queue_for_flash,_loopqueue_entry)									\
	,&CONCAT_2(p_handler,_flash_action)														\
	,&CONCAT_2(p_handler,_flash_timer_entry)												\
	,&CONCAT_2(p_handler,_s_MFEHS)															\
	,debug_method																			\
	,param_debug																			\
	};																						\
	static sMyFlashEventHandler const* p_handler = &CONCAT_2(p_handler,_flash_handle_entry)
#endif



m_app_result mFlashEventInit(const sMyFlashEventHandler *p_handler);
m_app_result mFlashEventUnInit(const sMyFlashEventHandler *p_handler);
X_Void 		 mFlashEventHandlerRun(const sMyFlashEventHandler *p_handler);

m_app_result mFlashReadRequest(const sMyFlashEventHandler *p_handler,uint32_t read_start_addr,uint32_t read_length,FLASH_POINTER_TYPE  * p_dest,onMyFlashRead read_cb);
m_app_result mFlashWriteRequest(const sMyFlashEventHandler *p_handler,uint32_t write_start_addr,uint32_t write_length,FLASH_POINTER_TYPE const * p_src,onMyFlashWrite write_cb);
m_app_result mFlashEraseRequest(const sMyFlashEventHandler *p_handler,uint32_t erase_start_addr,uint32_t erase_length,onMyFlashErase erase_cb);

#if (M_FLASH_ENABLE_USER_MULTI_PARTITION == 1)
m_app_result mFlashSectorReadRequest(const sMyFlashEventHandler *p_handler,uint32_t page_number,uint32_t sector_number,uint32_t length,FLASH_POINTER_TYPE * p_dest,onMyFlashRead read_cb);
m_app_result mFlashSectorWriteRequest(const sMyFlashEventHandler *p_handler,uint32_t page_number,uint32_t sector_number,uint32_t length,FLASH_POINTER_TYPE const * p_src,onMyFlashWrite write_cb);
#endif
m_app_result mFlashEventHandlerHold(const sMyFlashEventHandler *p_handler);
m_app_result mFlashEventHandlerBeginProcessing(const sMyFlashEventHandler *p_handler);
X_Boolean DoesMyFlashModuleBusy(const sMyFlashEventHandler *p_handler);


#endif
