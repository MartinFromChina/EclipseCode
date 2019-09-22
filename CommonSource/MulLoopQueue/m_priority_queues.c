#include "m_priority_queues.h"
/*
 ***********************************************************************************************
 *1,push node into queue with priority
 *if same priority is pushed ,the previous one will be replace by the new one
 *2,pop node by priority (from small to large or from large to small could be config)
 *3,user can check the queue state include : (is empty ? min priority ,max priority , cunrrent used node number and so on)
 ***********************************************************************************************
 */
X_Void 		mPriorityQueueInitialize(const sMyPriorityListManager *p_manager)
{

}
X_Boolean   mPriorityQueuePush(const sMyPriorityListManager *p_manager,sMyPriorityNodeParam const *p_data)
{

}
X_Boolean   mPriorityQueuePop(const sMyPriorityListManager *p_manager,sMyPriorityNodeParam *p_data)
{

}
X_Void      ClearMyPriorityQueue(const sMyPriorityListManager *p_manager)
{

}
X_Void      RealseMyPriorityQueueNode(const sMyPriorityListManager *p_manager,uint8_t buf_num)
{

}
uint16_t    GetMyPriorityQueueUsedNodeCount(const sMyPriorityListManager *p_manager)
{

}
X_Boolean   GetCurrentUsedPriorityScope(const sMyPriorityListManager *p_manager,uint16_t *p_min,uint16_t *p_max)
{

}
X_Boolean   DoesMyPriorityQueueEmpty(const sMyPriorityListManager *p_manager)
{

}
