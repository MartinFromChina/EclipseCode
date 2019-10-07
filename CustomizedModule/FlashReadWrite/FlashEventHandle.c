#include "FlashEventHandle.h"
/*
 ******************************************************************************************
 handle flash read write event,descripe in detail:
 1, handle basic  read or write request in queue(the queue only hold the buf pointer)
 2, handle user define sector base on MCU sector system
 ******************************************************************************************
 */
#if (M_FLASH_ENABLE_PARAM_CHECK == 1)
static X_Boolean does_addr_is_page_aligned(uint32_t erase_unit,uint32_t addr)
{
    return (addr & (erase_unit - 1)) == 0;
}
static X_Boolean does_addr_is_aligned32(uint32_t addr)
{
    return !(addr & 0x03);
}
static X_Boolean does_addr_is_within_bounds(uint32_t start_addr,uint32_t total_length
											,uint32_t addr,uint32_t len)
{
    return (   (addr           >= start_addr)
            && ((addr + len) <= (start_addr + total_length))  );
}
#endif


m_app_result mFlashEventInit(const sMyFlashEventHandler *p_handler)
{
	if(p_handler == X_Null) {return APP_POINTER_NULL;}
	if(p_handler->p_action ->init == X_Null) {return APP_POINTER_NULL;}
//	if(p_handler->p_action ->uninit == X_Null) {return APP_POINTER_NULL;} // not necessary
	if(p_handler->p_action ->read_evt_handler == X_Null) {return APP_POINTER_NULL;}
	if(p_handler->p_action ->write_evt_handler == X_Null) {return APP_POINTER_NULL;}
	if(p_handler->p_action ->erase_evt_handler == X_Null) {return APP_POINTER_NULL;}
	if(p_handler->p_action ->doesbusy == X_Null) {return APP_POINTER_NULL;}

	#if (M_FLASH_ENABLE_PARAM_CHECK == 1)
		if(p_handler ->p_basic_param->total_size_in_bytes == 0) {return APP_PARAM_ERROR;}
		if(p_handler ->p_basic_param->erase_uint == 0 || p_handler ->p_basic_param->program_unit == 0) {return APP_PARAM_ERROR;}
		if(does_addr_is_page_aligned(p_handler ->p_basic_param ->erase_uint,p_handler ->p_basic_param ->base_addr) == X_False) {return APP_PARAM_ERROR;}
		#if (M_FLASH_ENABLE_USER_MULTI_PARTITION == 1)
			if(p_handler ->p_basic_param->total_size_in_bytes !=
				(p_handler ->p_user_param ->sector_size * p_handler ->p_user_param ->total_sector_count)){return APP_PARAM_ERROR;}
		#endif
	#endif
	p_handler->p_action ->init();
	p_handler->p_manager->priority_counter = 0;
	mPriorityQueueInitialize(p_handler ->p_priority_queue);
	p_handler ->p_manager ->isInitOK = X_True;

	#if (USE_MY_FLASH_EVENT_HANDLER_DEBUG == 1)
	if(p_handler ->onDebug != X_Null)
	{
		if(p_handler ->onDebugParamCollect != X_Null)
		{
			p_handler ->onDebugParamCollect(SQO_Push,p_handler ->p_basic_param ->base_addr);
			p_handler ->onDebugParamCollect(SQO_Push,p_handler ->p_basic_param ->total_size_in_bytes);
			p_handler ->onDebugParamCollect(SQO_Push,p_handler ->p_basic_param ->erase_uint);
			p_handler ->onDebugParamCollect(SQO_Push,p_handler ->p_basic_param ->program_unit);

			p_handler ->onDebugParamCollect(SQO_Push,p_handler ->p_user_param ->sector_size);
			p_handler ->onDebugParamCollect(SQO_Push,p_handler ->p_user_param ->total_sector_count);
		}
		p_handler ->onDebug(FO_Init,0,p_handler);
		if(p_handler ->onDebugParamCollect != X_Null){p_handler ->onDebugParamCollect(SQO_Clear,0);}
	}
	#endif
	return APP_SUCCESSED;
}
m_app_result mFlashEventUnInit(const sMyFlashEventHandler *p_handler)
{
	if(p_handler == X_Null) {return APP_POINTER_NULL;}
	if(p_handler ->p_manager ->isInitOK == X_False) {return APP_UNEXPECT_STATE;}
	if(p_handler->p_action ->uninit != X_Null) {p_handler->p_action ->uninit();}

	p_handler->p_manager->priority_counter = 0;
	ClearSimpleQueue(p_handler ->p_loop_queue);
	ClearMyPriorityQueue(p_handler ->p_priority_queue);
	p_handler ->p_manager ->isInitOK = X_False;
	return APP_SUCCESSED;
}
m_app_result mFlashReadRequest(const sMyFlashEventHandler *p_handler,uint32_t read_start_addr,uint32_t read_length,FLASH_POINTER_TYPE * p_dest,onMyFlashRead read_cb)
{
	uint16_t node_num;
	X_Boolean isOK;

	if(p_handler == X_Null) {return APP_POINTER_NULL;}
	if(p_handler ->p_manager ->isInitOK == X_False) {return APP_UNEXPECT_STATE;}

#if (M_FLASH_ENABLE_PARAM_CHECK == 1)
	if(does_addr_is_aligned32(read_start_addr) == X_False) {return APP_PARAM_ERROR;}
	if(read_length == 0) {return APP_PARAM_ERROR;}
	if(does_addr_is_within_bounds(p_handler ->p_basic_param ->base_addr,p_handler ->p_basic_param ->total_size_in_bytes
									,read_start_addr,read_length)== X_False) {return APP_PARAM_ERROR;}
#endif
	M_FLASH_ENTER_CRITICAL_REGION_METHOD;
	node_num = SimpleQueueFirstIn(p_handler ->p_loop_queue,&isOK,X_False);
	M_FLASH_EXIT_CRITICAL_REGION_METHOD;
	if(isOK == X_True)
	{
		p_handler ->p_manager ->fr[node_num].read_start_addr = read_start_addr;
		p_handler ->p_manager ->fr[node_num].read_length     = read_length;
		p_handler ->p_manager ->fr[node_num].p_dest			 = p_dest;
		p_handler ->p_manager ->fr[node_num].read_cb		 = read_cb;
		return APP_SUCCESSED;
	}
	return APP_UNEXPECT_STATE;
}
m_app_result mFlashWriteRequest(const sMyFlashEventHandler *p_handler,uint32_t write_start_addr,uint32_t write_length,FLASH_POINTER_TYPE const * p_src,onMyFlashWrite write_cb)
{
	uint16_t node_num;
	X_Boolean isOK;

	if(p_handler == X_Null) {return APP_POINTER_NULL;}
	if(p_handler ->p_manager ->isInitOK == X_False) {return APP_UNEXPECT_STATE;}

#if (M_FLASH_ENABLE_PARAM_CHECK == 1)
	if(write_length == 0) {return APP_PARAM_ERROR;}
	/* Length must be a multiple of the program unit. */
	if(write_length % p_handler ->p_basic_param ->program_unit != 0) {return APP_PARAM_ERROR;}
	/* Source and destination addresses must be word-aligned. */
	if(does_addr_is_aligned32(write_start_addr) == X_False) {return APP_PARAM_ERROR;}
	if(does_addr_is_aligned32((uint32_t)p_src) == X_False) {return APP_PARAM_ERROR;}

	if(does_addr_is_within_bounds(p_handler ->p_basic_param ->base_addr,p_handler ->p_basic_param ->total_size_in_bytes
										,write_start_addr,write_length)== X_False) {return APP_PARAM_ERROR;}
#endif

	M_FLASH_ENTER_CRITICAL_REGION_METHOD;
	isOK = mPriorityQueuePush(p_handler ->p_priority_queue,p_handler->p_manager->priority_counter,&node_num);
	p_handler->p_manager->priority_counter ++;
	M_FLASH_EXIT_CRITICAL_REGION_METHOD;

	if(isOK == X_True)
	{
		p_handler ->p_manager ->fw[node_num].write_start_addr = write_start_addr;
		p_handler ->p_manager ->fw[node_num].write_length     = write_length;
		p_handler ->p_manager ->fw[node_num].p_src			  = p_src;
		p_handler ->p_manager ->fw[node_num].write_cb		  = write_cb;
		return APP_SUCCESSED;
	}
	return APP_UNEXPECT_STATE;
}
m_app_result mFlashEraseRequest(const sMyFlashEventHandler *p_handler,uint32_t erase_start_addr,uint32_t erase_length,onMyFlashErase erase_cb)
{
	if(p_handler == X_Null) {return APP_POINTER_NULL;}
	if(p_handler ->p_manager ->isInitOK == X_False) {return APP_UNEXPECT_STATE;}

#if (M_FLASH_ENABLE_PARAM_CHECK == 1)
	if(does_addr_is_page_aligned(p_handler ->p_basic_param ->erase_uint,erase_start_addr) == X_False) {return APP_PARAM_ERROR;}
	if(erase_length == 0) {return APP_PARAM_ERROR;}
	if(does_addr_is_within_bounds(p_handler ->p_basic_param ->base_addr,p_handler ->p_basic_param ->total_size_in_bytes
									,erase_start_addr,erase_length)== X_False) {return APP_PARAM_ERROR;}
#endif
	p_handler ->p_action ->erase_evt_handler(erase_start_addr,erase_length);//erase_evt_handler != X_Null is checked when init
	return APP_SUCCESSED;
}

#if (M_FLASH_ENABLE_USER_MULTI_PARTITION == 1)
m_app_result mFlashSectorReadRequest(const sMyFlashEventHandler *p_handler,uint32_t sector_number,uint32_t length,FLASH_POINTER_TYPE  * p_dest,onMyFlashRead read_cb)
{
	if(p_handler == X_Null) {return APP_POINTER_NULL;}
	if(p_handler ->p_manager ->isInitOK == X_False) {return APP_UNEXPECT_STATE;}

#if (M_FLASH_ENABLE_PARAM_CHECK == 1)
#endif
	return APP_SUCCESSED;
}
m_app_result mFlashSectorWriteRequest(const sMyFlashEventHandler *p_handler,uint32_t sector_number,uint32_t length,FLASH_POINTER_TYPE const * p_src,onMyFlashWrite write_cb)
{
	if(p_handler == X_Null) {return APP_POINTER_NULL;}
	if(p_handler ->p_manager ->isInitOK == X_False) {return APP_UNEXPECT_STATE;}

#if (M_FLASH_ENABLE_PARAM_CHECK == 1)
#endif

	return APP_SUCCESSED;
}

m_app_result mFlashEventHandlerHold(const sMyFlashEventHandler *p_handler)
{
	if(p_handler == X_Null) {return APP_POINTER_NULL;}
	if(p_handler ->p_manager ->isInitOK == X_False) {return APP_UNEXPECT_STATE;}
	return APP_SUCCESSED;
}
m_app_result mFlashEventHandlerBeginProcessing(const sMyFlashEventHandler *p_handler)
{
	if(p_handler == X_Null) {return APP_POINTER_NULL;}
	if(p_handler ->p_manager ->isInitOK == X_False) {return APP_UNEXPECT_STATE;}
	return APP_SUCCESSED;
}
#endif

X_Void mFlashEventHandlerRun(const sMyFlashEventHandler *p_handler)
{
	s_StateMachineParam smp;
	if(p_handler == X_Null) {return;}
	if(p_handler ->p_manager ->isInitOK == X_False) {return;}

	SimpleStateMachineRun(p_handler->p_flash_state,&smp,X_Null,X_Null);

}
