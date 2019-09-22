#include "m_priority_queues.h"
/*
 ***********************************************************************************************
 *1,push node into queue with priority
 *if same priority is pushed ,the previous one will be replace by the new one
 *2,pop node by priority (from small to large or from large to small could be config)
 *3,user can check the queue state include : (is empty ? min priority ,max priority , cunrrent used node number and so on)
 ***********************************************************************************************
 */
typedef enum
{
	QP_Replace,
	QP_Insert,
}ePriorityQueuePushAction;

static X_Boolean GetActionAndNodeNumber(const sMySingleLinkList * s_sll,X_Boolean isFromSmall,uint16_t priority
									,ePriorityQueuePushAction *p_action,uint16_t *p_number)
{

}
X_Void 		mPriorityQueueInitialize(const sMyPriorityListManager *p_manager)
{
	uint16_t i;
	if(p_manager == X_Null) {return;}
	mSingleListInit(p_manager ->p_list);
	if(p_manager ->MaxNodeNumber >= 500) {return;}

	for(i=0;i<p_manager ->MaxNodeNumber;i++)
	{
		p_manager ->p_param[i].is_OccupyPermit = X_True;
		p_manager ->p_param[i].priority		   = 0;
		p_manager ->p_param[i].p_buf           = X_Null;
	}
	*p_manager ->isInit = X_True;
}
X_Boolean   mPriorityQueuePush(const sMyPriorityListManager *p_manager,sMyPriorityNodeParam const *p_data)
{
	uint16_t node_count,node_number;
	ePriorityQueuePushAction e_pqpa;
	if(p_manager == X_Null || p_data == X_Null) {return X_False;}
	if(*p_manager ->isInit == X_False) {return X_False;}
	if(p_data ->priority >= p_manager ->MaxNodeNumber){return X_False;}
	if(p_data ->p_buf == X_Null)	 {return X_False;}
	if( mSingleListSizeGet(p_manager->p_list,&node_count) == X_False){return X_False;}

	p_manager ->p_param[p_data ->priority].is_OccupyPermit = p_data ->is_OccupyPermit;
	p_manager ->p_param[p_data ->priority].priority = p_data ->priority;
	p_manager ->p_param[p_data ->priority].p_buf = p_data ->p_buf;
	if(node_count == 0)// insert first element
	{
		if(mSingleListHeadAdd(p_manager->p_list,p_data ->priority) == APP_SUCCESSED){return X_True;}
		return X_False;
	}
	else if(node_count >= p_manager ->MaxNodeNumber) // only replace can be done
	{
		if(GetActionAndNodeNumber(p_manager->p_list,p_manager ->isHighPriorityFromSmall,p_data ->priority,&e_pqpa,&node_number) == X_True)
		{
			if(e_pqpa == QP_Insert) {return X_False;}
			if(mSingleListUpdataValueByNodeNumber(p_manager->p_list,node_number,p_data ->priority) == X_True) {return X_True;}
			return X_False;
		}
		return X_False;
	}
	else
	{
		if(GetActionAndNodeNumber(p_manager->p_list,p_manager ->isHighPriorityFromSmall,p_data ->priority,&e_pqpa,&node_number) == X_True)
		{
			if(e_pqpa == QP_Insert)
			{
				if(mSingleListInsert(p_manager->p_list,node_number,p_data ->priority) == APP_SUCCESSED){return X_True;}
				return X_False;
			}
			if(mSingleListUpdataValueByNodeNumber(p_manager->p_list,node_number,p_data ->priority) == X_True) {return X_True;}
			return X_False;
		}
		return X_False;
	}
}
X_Boolean   mPriorityQueuePop(const sMyPriorityListManager *p_manager,sMyPriorityNodeParam *p_data)
{
	uint16_t infor_number;
	if(p_manager == X_Null) {return X_False;}
	if(*p_manager ->isInit == X_False) {return X_False;}
	if(DoesMySingleListEmpty(p_manager ->p_list) == X_True)  {return X_False;}

	if(mSingleListInformationGetByNodeNumber(p_manager ->p_list,0,&infor_number) == X_True)
	{
		mSingleListHeadRemove(p_manager ->p_list);
		if(infor_number >= p_manager ->MaxNodeNumber) {return X_False;}
		p_data ->is_OccupyPermit = p_manager ->p_param[infor_number].is_OccupyPermit;
		p_data ->priority		 = p_manager ->p_param[infor_number].priority;
		p_data ->p_buf			 = p_manager ->p_param[infor_number].p_buf;
		return X_True;
	}
	return X_False;
}
X_Void    ClearMyPriorityQueue(const sMyPriorityListManager *p_manager)
{
	mPriorityQueueInitialize(p_manager);
}
m_app_result    RealseMyPriorityQueueNode(const sMyPriorityListManager *p_manager,uint16_t node_priority)
{
	uint16_t node_number;
	if(p_manager == X_Null) {return APP_POINTER_NULL;}
	if(*p_manager ->isInit == X_False) {return APP_ERROR;}

	if(mSingleListFindByValue(p_manager ->p_list,node_priority,&node_number) == X_True)
	{
		return mSingleListPullAway(p_manager ->p_list,node_number);
	}
	return APP_ERROR;
}
uint16_t    GetMyPriorityQueueUsedNodeCount(const sMyPriorityListManager *p_manager)
{
	uint16_t node_count;
	if(p_manager == X_Null) {return 0;}
	if(*p_manager ->isInit == X_False) {return 0;}
	if( mSingleListSizeGet(p_manager->p_list,&node_count) == X_True){return node_count;}
	return 0;
}
X_Boolean   GetCurrentUsedPriorityScope(const sMyPriorityListManager *p_manager,uint16_t *p_high,uint16_t *p_low)
{
	uint16_t node_count;
	if(p_manager == X_Null) {return X_False;}
	if(*p_manager ->isInit == X_False) {return X_False;}

	if(DoesMySingleListEmpty(p_manager ->p_list) == X_True){return X_False;}

	if(mSingleListInformationGetByNodeNumber(p_manager ->p_list,0,p_high) == X_False) {return X_False;}
	if(mSingleListSizeGet(p_manager->p_list,&node_count) == X_False) {return X_False;}
	if(mSingleListInformationGetByNodeNumber(p_manager ->p_list,node_count,p_low) == X_False) {return X_False;}
	return X_True;
}
X_Boolean   DoesMyPriorityQueueEmpty(const sMyPriorityListManager *p_manager)
{
	if(p_manager == X_Null) {return X_False;}
	if(*p_manager ->isInit == X_False) {return X_False;}
	return DoesMySingleListEmpty(p_manager ->p_list);
}
