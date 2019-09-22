#ifndef __M_PRIORITY_QUEUES_H
#define __M_PRIORITY_QUEUES_H

/****************************************************
 * careful about the thread-safe
 */

#include "stdbool.h"
#include "..\KeilMDK.h"
#include "m_list_node.h"

#define MAX_PRIORITY_QUEUE_COUNT      500

typedef struct
{
	X_Boolean 	is_OccupyPermit; // false : can't be replaced by new data  with the same priority
	uint16_t     priority;
	uint8_t     *p_buf;
}sMyPriorityNodeParam;

typedef struct
{
	X_Boolean 			*isInit;
    sMySingleLinkList 	const*p_list;
	uint16_t    		MaxNodeNumber;
	X_Boolean			isHighPriorityFromSmall;// priority from high to low ;  true : the smaller value ,the high priority ; false : the bigger value ,the high priority
    sMyPriorityNodeParam *p_param;
}sMyPriorityListManager;

#define APP_PRIORITY_QUEUE_DEF(p_manager,max_node_number,is_from_small)            					\
		static X_Boolean CONCAT_2(p_manager,_isInit) = X_False;										\
		APP_SINGLE_LIST_DEF_WITHOUT_POINTER(p_manager,max_node_number);    							\
		static sMyPriorityNodeParam CONCAT_2(p_manager,_queue_param)[max_node_number];				\
		static  const sMyPriorityListManager  CONCAT_2(p_manager,_queue_entry) = {					\
			&CONCAT_2(p_manager,_isInit),															\
			&CONCAT_2(p_manager,_list_entry),														\
			max_node_number,																		\
			is_from_small,																			\
			CONCAT_2(p_manager,_queue_param),														\
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


#endif
