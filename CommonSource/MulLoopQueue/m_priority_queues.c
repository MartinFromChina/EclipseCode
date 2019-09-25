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

static X_Boolean GetActionAndNodeNumber(const sMySingleLinkList * s_sll,X_Boolean isFromSmall,uint16_t priority,uint16_t current_node_count
									,ePriorityQueuePushAction *p_action,uint16_t *p_number)
{
	uint16_t i,infor_number;
//	X_Boolean isOK;
	if(s_sll == X_Null ||p_action == X_Null || p_number == X_Null){return X_False;}
	if(current_node_count > s_sll ->ValidNodeNumber){return X_False;}
	if(current_node_count == 0)
	{
		*p_action =	QP_Insert;
		*p_number = 0;
		return X_True;
	}

	if(isFromSmall == X_True)
	{
		for(i=0;i<current_node_count;i++)
		{
			if(mSingleListInformationGetByNodeNumber(s_sll,i,&infor_number) == X_True)
			{
				if(priority == infor_number)
				{
					*p_action = QP_Replace;
					*p_number = i;
					return X_True;
				}
				else if(priority < infor_number )
				{
					*p_action = QP_Insert;
					*p_number = i;
					return X_True;
				}
			}
			else
			{
				return X_False;
			}
		}
		*p_action = QP_Insert;
		*p_number = i;
		return X_True;
	}
	else
	{
		for(i=0;i<current_node_count;i++)
		{
			if(mSingleListInformationGetByNodeNumber(s_sll,i,&infor_number) == X_True)
			{
				if(priority == infor_number)
				{
					*p_action = QP_Replace;
					*p_number = i;
					return X_True;
				}
				else if(priority > infor_number )
				{
					*p_action = QP_Insert;
					*p_number = i;
					return X_True;
				}
			}
			else
			{
				return X_False;
			}
		}
		*p_action = QP_Insert;
		*p_number = i;
		return X_True;
	}
}
X_Void 		mPriorityQueueInitialize(const sMyPriorityListManager *p_manager)
{
	uint16_t i,priority_default;
	if(p_manager == X_Null) {return;}
	mSingleListInit(p_manager ->p_list);
	if(p_manager ->MaxNodeNumber >= 500) {return;}

	priority_default = (p_manager ->isHighPriorityFromSmall == X_True) ? MY_MAX_PRIORITY_VALUE : 0;

	for(i=0;i<p_manager ->MaxNodeNumber;i++)
	{
		p_manager ->p_param[i].is_OccupyPermit = X_True;
		p_manager ->p_param[i].priority		   = priority_default;
		p_manager ->p_param[i].p_buf           = X_Null;
	}
	*p_manager ->isInit = X_True;
	if(p_manager ->onDebug != X_Null) {p_manager ->onDebug(PQO_init,0,p_manager);}
}
X_Boolean   mPriorityQueuePush(const sMyPriorityListManager *p_manager,sMyPriorityNodeParam const *p_data)
{
	uint8_t error_code;
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
		if(p_manager ->onDebug != X_Null)
		{
			if(p_manager ->onDebugParamCollect != X_Null) {p_manager ->onDebugParamCollect(SQO_Push,0);}
			if(p_manager ->onDebugParamCollect != X_Null) {p_manager ->onDebugParamCollect(SQO_Push,p_data ->priority);}
			p_manager ->onDebug(PQO_Push,0,p_manager);
			if(p_manager ->onDebugParamCollect != X_Null) {p_manager ->onDebugParamCollect(SQO_Clear,0);}
		}
		if(mSingleListHeadAdd(p_manager->p_list,p_data ->priority) == APP_SUCCESSED){return X_True;}
		return X_False;
	}
	else if(node_count >= p_manager ->MaxNodeNumber) // only replace can be done
	{
		if(p_manager ->onDebug != X_Null) {p_manager ->onDebug(PQO_Push,1,p_manager);}
		if(GetActionAndNodeNumber(p_manager->p_list,p_manager ->isHighPriorityFromSmall,p_data ->priority,node_count
									,&e_pqpa,&node_number) == X_True)
		{
			if(e_pqpa == QP_Insert) {return X_False;}
			if(mSingleListUpdataValueByNodeNumber(p_manager->p_list,node_number,p_data ->priority) == X_True) {return X_True;}
			return X_False;
		}
		return X_False;
	}
	else
	{
		if(GetActionAndNodeNumber(p_manager->p_list,p_manager ->isHighPriorityFromSmall,p_data ->priority,node_count
									,&e_pqpa,&node_number) == X_True)
		{
			if(p_manager ->onDebug != X_Null)
			{
				if(p_manager ->onDebugParamCollect != X_Null) {p_manager ->onDebugParamCollect(SQO_Push,node_number);}
				if(p_manager ->onDebugParamCollect != X_Null) {p_manager ->onDebugParamCollect(SQO_Push,p_data ->priority);}
				p_manager ->onDebug(PQO_Push,2,p_manager);
				if(p_manager ->onDebugParamCollect != X_Null) {p_manager ->onDebugParamCollect(SQO_Clear,0);}
			}
			if(e_pqpa == QP_Insert)
			{
				error_code = mSingleListInsert(p_manager->p_list,node_number,p_data ->priority);
				if( error_code == APP_SUCCESSED){return X_True;}
				if(p_manager ->onDebug != X_Null)
				{
					if(p_manager ->onDebugParamCollect != X_Null) {p_manager ->onDebugParamCollect(SQO_Push,error_code);}
					p_manager ->onDebug(PQO_Push,3,p_manager);
					if(p_manager ->onDebugParamCollect != X_Null) {p_manager ->onDebugParamCollect(SQO_Clear,0);}
				}
				return X_False;
			}
			if(mSingleListUpdataValueByNodeNumber(p_manager->p_list,node_number,p_data ->priority) == X_True) {return X_True;}
			if(p_manager ->onDebug != X_Null) {p_manager ->onDebug(PQO_Push,4,p_manager);}
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
