#ifndef __M_PRIORITY_QUEUES_H
#define __M_PRIORITY_QUEUES_H

/****************************************************
 * careful about the thread-safe
 */

#include "stdbool.h"
#include "..\KeilMDK.h"
#include "m_list_node.h"

#ifdef USE_LOCAL_PRIORITY_QUEUE_CONFIG
	#include "m_priority_queues_config_local.h"
#else
	#include "m_priority_queues_config_default.h"
#endif

typedef enum
{
	PQO_init = 0,
	PQO_Push,
	PQO_Pop,
	PQO_Clear,
	PQO_ReleaseNode,
	PQO_NodeCount,
	PQO_PriorityScope,
	PQO_DoesEmpty,
}ePriorityQueueOperation;

typedef struct
{
	X_Boolean 	is_OccupyPermit; // false : can't be replaced by new data  with the same priority
	uint16_t     priority;
	uint8_t     *p_buf;
}sMyPriorityNodeParam;

typedef struct s_MyPriorityListManager sMyPriorityListManager;

struct s_MyPriorityListManager
{
	X_Boolean 			*isInit;
    sMySingleLinkList 	const*p_list;
	uint16_t    		MaxNodeNumber;
	X_Boolean			isHighPriorityFromSmall;// priority from high to low ;  true : the smaller value ,the high priority ; false : the bigger value ,the high priority
    sMyPriorityNodeParam *p_param;
    X_Void (*onDebug)(ePriorityQueueOperation e_ppo,uint8_t operation_ID,const sMyPriorityListManager *p_lm);
};

typedef   X_Void (*m_priority_queue_debug_handler)(const sMyPriorityListManager *p_lm);

typedef struct
{
	ePriorityQueueOperation ePQO;
	m_priority_queue_debug_handler debug_handler[MAX_PRIORITY_QUEUE_DEBUG_ID_COUNT];
}sPriorityQueueMessageDebugTable;

#define APP_PRIORITY_QUEUE_DEF(p_manager,max_node_number,is_from_small,list_debug,queue_debug)      \
		static X_Boolean CONCAT_2(p_manager,_isInit) = X_False;										\
		APP_SINGLE_LIST_DEF_WITHOUT_POINTER(p_manager,max_node_number,list_debug);    				\
		static sMyPriorityNodeParam CONCAT_2(p_manager,_queue_param)[max_node_number];				\
		static  const sMyPriorityListManager  CONCAT_2(p_manager,_queue_entry) = {					\
			&CONCAT_2(p_manager,_isInit),															\
			&CONCAT_2(p_manager,_list_entry),														\
			max_node_number,																		\
			is_from_small,																			\
			CONCAT_2(p_manager,_queue_param),														\
			queue_debug,																			\
		} ;																						    \
		static  sMyPriorityListManager const* p_manager = &CONCAT_2(p_manager,_queue_entry)

X_Void 		mPriorityQueueInitialize(const sMyPriorityListManager *p_manager);
X_Boolean   mPriorityQueuePush(const sMyPriorityListManager *p_manager,sMyPriorityNodeParam const *p_data);
X_Boolean   mPriorityQueuePop(const sMyPriorityListManager *p_manager,sMyPriorityNodeParam *p_data);
X_Void      ClearMyPriorityQueue(const sMyPriorityListManager *p_manager);
m_app_result  RealseMyPriorityQueueNode(const sMyPriorityListManager *p_manager,uint16_t node_priority);
uint16_t    GetMyPriorityQueueUsedNodeCount(const sMyPriorityListManager *p_manager);
X_Boolean   GetCurrentUsedPriorityScope(const sMyPriorityListManager *p_manager,uint16_t *p_high,uint16_t *p_low);
X_Boolean   DoesMyPriorityQueueEmpty(const sMyPriorityListManager *p_manager);

/*
 * example debug code
 static const sPriorityQueueMessageDebugTable sPQMDT[] = {
		{
			PQO_init,
			{X_Null,X_Null,X_Null,X_Null,X_Null,},
		},
		{
			PQO_Push,
			{X_Null,X_Null,X_Null,X_Null,X_Null,},
		},
};

static X_Void onQueueDebug(ePriorityQueueOperation e_ppo,uint8_t operation_ID,const sMyPriorityListManager *p_lm)
{
	if(e_ppo >= (sizeof(sPQMDT)/sizeof(sPQMDT[0]))) {return;}
	if(operation_ID >= MAX_LISTNODE_DEBUG_ID_COUNT) {return;}
	if(sPQMDT[e_ppo].debug_handler[operation_ID] != X_Null) {sPQMDT[e_ppo].debug_handler[operation_ID](p_lm);}
}

 */
#endif
