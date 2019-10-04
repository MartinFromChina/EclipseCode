#ifndef __M_PRIORITY_QUEUES_H
#define __M_PRIORITY_QUEUES_H

/****************************************************
 * careful about the thread-safe
 */

#include "stdbool.h"
#include "..\KeilMDK.h"
#include "..\AppError.h"
#include "..\AppCommon.h"
#include "loop_queues.h"

#ifdef USE_LOCAL_PRIORITY_QUEUE_CONFIG
	#include "m_priority_queues_config_local.h"
#else
	#include "m_priority_queues_config_default.h"
#endif

/*
 * scope : 0 ~ max_priority
 * table size = (scope % 32 == 0) ? scope / 32 : scope / 32 + 1
 * but the conditional judgment (scope % 32 == 0) ? is not a good way here
 * so I lost the space to exchange sth : table size =  scope / 32 + 1;
 *
 */
#define GET_PRIORITY_TABLE_SIZE_BY_PRIORITY_SCOPE(scope)    (uint16_t)((scope/BIT_COUNT_IN_UINT32) + 1)

typedef struct _sListNodeWithPriority sListNodeWithPriority;
struct _sListNodeWithPriority
{
	uint16_t    node_ID_in_list_array;
	X_Boolean 	isFree;
	uint16_t    priority;
	sListNodeWithPriority   *NextPtr;
};

typedef struct
{
	uint16_t const max_node_count;
	uint16_t  *p_current_used_node_count;
	sListNodeWithPriority **p_list_head_nodes;
	sListNodeWithPriority *p_space;
}sPriorityQueueListNodeSpace;


typedef struct s_MyPriorityListManager sMyPriorityListManager;

struct s_MyPriorityListManager
{
	X_Boolean 			*isInit;
	uint16_t    		MaxNodeCount;
	uint16_t            MaxPriorityValue;
	X_Boolean			isHighPriorityFromSmall;// priority from high to low ;  true : the smaller value ,the high priority ; false : the bigger value ,the high priority
    uint32_t            *p_priority_table;
    const sPriorityQueueListNodeSpace *p_node_space_handle;
};

//priority:0~max_value,if the priority scope is extremely big and its min value is not 0 , offset it to 0 to save space
#define APP_PRIORITY_QUEUE_DEF_WITHOUT_POINTER(p_manager											\
								,max_node_number													\
								,max_priority_value													\
								,is_from_small														\
								)  																	\
		static uint32_t  CONCAT_2(p_manager,_priority_table)[GET_PRIORITY_TABLE_SIZE_BY_PRIORITY_SCOPE(max_priority_value)]; \
		static X_Boolean CONCAT_2(p_manager,_isInit) = X_False;										\
		static uint16_t  CONCAT_2(p_manager,_current_priority_queue_used_node_count) = 0;	 	    \
		static sListNodeWithPriority CONCAT_2(p_manager,_node_with_priority_space)[max_node_number];\
		static sListNodeWithPriority* CONCAT_2(p_manager,_head_nodes)[LIST_NODE_TABLE_SIZE];			\
		static const sPriorityQueueListNodeSpace CONCAT_2(p_manager,_node_with_priority_space_handle) = {	\
				max_node_number,																	\
				&CONCAT_2(p_manager,_current_priority_queue_used_node_count),						\
				CONCAT_2(p_manager,_head_nodes),													\
   	   	   	    CONCAT_2(p_manager,_node_with_priority_space),										\
		};																							\
		static  const sMyPriorityListManager  CONCAT_2(p_manager,_priority_queue_entry) = {			\
			&CONCAT_2(p_manager,_isInit),															\
			max_node_number,																		\
			max_priority_value,																		\
			is_from_small,																			\
			CONCAT_2(p_manager,_priority_table),													\
			&CONCAT_2(p_manager,_node_with_priority_space_handle),									\
		}

#define APP_PRIORITY_QUEUE_DEF(p_manager															\
								,max_node_number													\
								,max_priority_value													\
								,is_from_small														\
								)  																	\
		static uint32_t  CONCAT_2(p_manager,_priority_table)[GET_PRIORITY_TABLE_SIZE_BY_PRIORITY_SCOPE(max_priority_value)]; \
		static X_Boolean CONCAT_2(p_manager,_isInit) = X_False;										\
		static uint16_t  CONCAT_2(p_manager,_current_priority_queue_used_node_count) = 0;	 	    \
		static sListNodeWithPriority CONCAT_2(p_manager,_node_with_priority_space)[max_node_number];\
		static sListNodeWithPriority* CONCAT_2(p_manager,_head_nodes)[LIST_NODE_TABLE_SIZE];			\
		static const sPriorityQueueListNodeSpace CONCAT_2(p_manager,_node_with_priority_space_handle) = {	\
				max_node_number,																	\
				&CONCAT_2(p_manager,_current_priority_queue_used_node_count),						\
				CONCAT_2(p_manager,_head_nodes),													\
   	   	   	    CONCAT_2(p_manager,_node_with_priority_space),										\
		};																							\
		static  const sMyPriorityListManager  CONCAT_2(p_manager,_priority_queue_entry) = {			\
			&CONCAT_2(p_manager,_isInit),															\
			max_node_number,																		\
			max_priority_value,																		\
			is_from_small,																			\
			CONCAT_2(p_manager,_priority_table),													\
			&CONCAT_2(p_manager,_node_with_priority_space_handle),									\
		} ;																						    \
		static  sMyPriorityListManager const* p_manager = &CONCAT_2(p_manager,_priority_queue_entry)

X_Void 		mPriorityQueueInitialize(const sMyPriorityListManager *p_manager);
X_Boolean   mPriorityQueuePush(const sMyPriorityListManager *p_manager,uint16_t priority,uint16_t *p_node_number);
X_Boolean   mPriorityQueuePop(const sMyPriorityListManager *p_manager,uint16_t *p_priority,uint16_t *p_node_number);
X_Void      ClearMyPriorityQueue(const sMyPriorityListManager *p_manager);
X_Boolean   RealseMyPriorityQueueNodeByPriority(const sMyPriorityListManager *p_manager,uint16_t priority,uint16_t *p_node_released);
uint16_t    GetMyPriorityQueueUsedNodeCount(const sMyPriorityListManager *p_manager);
X_Boolean   GetCurrentUsedPriorityScope(const sMyPriorityListManager *p_manager,uint16_t *p_high,uint16_t *p_low);
X_Boolean   GetPriorityByNodeNumber(const sMyPriorityListManager *p_manager,uint16_t node_number,uint16_t *p_priority);
X_Boolean   GetNodeNumberByPriority(const sMyPriorityListManager *p_manager,uint16_t priority,uint16_t *p_node_number);
X_Boolean   DoesMyPriorityQueueEmpty(const sMyPriorityListManager *p_manager);

#endif
