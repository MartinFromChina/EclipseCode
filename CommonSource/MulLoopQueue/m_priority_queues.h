#ifndef __M_PRIORITY_QUEUES_H
#define __M_PRIORITY_QUEUES_H

/****************************************************
 * careful about the thread-safe
 */

#include "stdbool.h"
#include "..\KeilMDK.h"

typedef struct
{
	X_Boolean 	is_OccupyPermit; // false : can't be replaced by new data  with the same priority
	uint16_t     priority;
	uint8_t     *p_buf;
}sMyPriorityNodeParam;

typedef struct
{
	uint8_t     *p_buf;
}sMyPriorityListManager;

X_Void 		mPriorityQueueInitialize(const sMyPriorityListManager *p_manager);
X_Boolean   mPriorityQueuePush(const sMyPriorityListManager *p_manager,sMyPriorityNodeParam const *p_data);
X_Boolean   mPriorityQueuePop(const sMyPriorityListManager *p_manager,sMyPriorityNodeParam *p_data);
X_Void      ClearMyPriorityQueue(const sMyPriorityListManager *p_manager);
X_Void      RealseMyPriorityQueueNode(const sMyPriorityListManager *p_manager,uint8_t buf_num);
uint16_t    GetMyPriorityQueueUsedNodeCount(const sMyPriorityListManager *p_manager);
X_Boolean   GetCurrentUsedPriorityScope(const sMyPriorityListManager *p_manager,uint16_t *p_min,uint16_t *p_max);
X_Boolean   DoesMyPriorityQueueEmpty(const sMyPriorityListManager *p_manager);


#endif
