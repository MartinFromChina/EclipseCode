#include "m_list_node.h"

static uint16_t NodeAddressMoveForward(uint16_t max_node_number,uint16_t current_address)
{
	if(current_address < (max_node_number - 1)){current_address ++;}
	else{current_address = 0;}
	return current_address;
}
static uint16_t NodeAddressStepBack(uint16_t max_node_number,uint16_t current_address)
{
	if(current_address > 0){current_address --;}
	else{current_address = (max_node_number - 1);}
	return current_address;
}
static uint16_t CurrentNodeAddressGet(uint16_t current_used_node_number,uint16_t max_node_number,uint16_t first_node_address)
{
	uint16_t i;
	uint16_t addr;

	addr = first_node_address;
	if(current_used_node_number == 0 || current_used_node_number == 1) {return first_node_address;}
	for(i=1;i<current_used_node_number;i++)
	{
		addr = NodeAddressMoveForward(max_node_number,addr);
	}
	return addr;
}

m_app_result mSingleListInit(const sMySingleLinkList * s_sll)
{
	uint16_t i;
	if(s_sll == X_Null) {return APP_POINTER_NULL;}
	if(s_sll->ValidNodeNumber > MY_MAX_NODE_COUNT) {return APP_BEYOND_SCOPE;}

	for(i=0;i<s_sll->ValidNodeNumber;i++)
	{
		s_sll ->p_inf_buf[i].next_node_address = MY_INVALID_NODE_CONTEXT;
		s_sll ->p_inf_buf[i].information_number = 0;
	}
	s_sll ->p_param ->first_node_address = 0;
	s_sll ->p_param ->used_node_num  = 0;
	return APP_SUCCESSED;
}
m_app_result mSingleListTailAdd(const sMySingleLinkList * s_sll,uint16_t infor_number)
{
	uint16_t  current_node_address,next_node_address;
	if(s_sll == X_Null) {return APP_POINTER_NULL;}
	if(s_sll ->p_param ->used_node_num >= s_sll->ValidNodeNumber) {return APP_BEYOND_SCOPE;}

	current_node_address = CurrentNodeAddressGet(s_sll ->p_param ->used_node_num,s_sll->ValidNodeNumber,s_sll ->p_param ->first_node_address);

	if(s_sll ->p_param ->used_node_num > 0)
	{
		next_node_address  = NodeAddressMoveForward(s_sll->ValidNodeNumber,current_node_address);
		s_sll ->p_inf_buf[current_node_address].next_node_address = next_node_address;
	}
	else
	{
		next_node_address    = current_node_address;
	}
	s_sll ->p_inf_buf[next_node_address].information_number = infor_number;
	s_sll ->p_param ->used_node_num ++;
	return APP_SUCCESSED;
}
m_app_result mSingleListTailRemove(const sMySingleLinkList * s_sll)
{
	uint16_t  current_node_address,previous_node_address;
	if(s_sll == X_Null) {return APP_POINTER_NULL;}
	if(s_sll ->p_param ->used_node_num > 0) //
	{
		current_node_address = CurrentNodeAddressGet(s_sll ->p_param ->used_node_num,s_sll->ValidNodeNumber,s_sll ->p_param ->first_node_address);
		if(s_sll ->p_param ->used_node_num == 1) //
		{
			previous_node_address = current_node_address;
		}
		else
		{
			previous_node_address = NodeAddressStepBack(s_sll->ValidNodeNumber,current_node_address);
		}
		s_sll ->p_inf_buf[previous_node_address].next_node_address = MY_INVALID_NODE_CONTEXT;
		s_sll ->p_param ->used_node_num --;
		return APP_SUCCESSED;
	}
	return APP_ALREADY_DONE_BEFORE;

}
m_app_result mSingleListHeadAdd(const sMySingleLinkList * s_sll,uint16_t infor_number)
{
	uint16_t  front_node_address;
	if(s_sll == X_Null) {return APP_POINTER_NULL;}
	if(s_sll ->p_param ->used_node_num >= s_sll->ValidNodeNumber) {return APP_BEYOND_SCOPE;}

	if(s_sll ->p_param ->used_node_num > 0)
	{
		front_node_address  = NodeAddressStepBack(s_sll->ValidNodeNumber,s_sll ->p_param ->first_node_address);
		s_sll ->p_inf_buf[front_node_address].next_node_address = s_sll ->p_param ->first_node_address;
	}
	else
	{
		front_node_address    = s_sll ->p_param ->first_node_address;
	}
	s_sll ->p_inf_buf[front_node_address].information_number = infor_number;
	s_sll ->p_param ->used_node_num ++;
	s_sll ->p_param ->first_node_address = front_node_address;
	return APP_SUCCESSED;
}
m_app_result mSingleListHeadRemove(const sMySingleLinkList * s_sll)
{
	uint16_t  next_node_address;
	if(s_sll == X_Null) {return APP_POINTER_NULL;}
	if(s_sll ->p_param ->used_node_num > 0) //
	{
		if(s_sll ->p_param ->used_node_num == 1) //
		{
			next_node_address = s_sll ->p_param ->first_node_address;
		}
		else
		{
			next_node_address = NodeAddressMoveForward(s_sll->ValidNodeNumber,s_sll ->p_param ->first_node_address);
		}
		s_sll ->p_inf_buf[s_sll ->p_param ->first_node_address].next_node_address = MY_INVALID_NODE_CONTEXT;
		s_sll ->p_param ->used_node_num --;
		s_sll ->p_param ->first_node_address = next_node_address;
		return APP_SUCCESSED;
	}
	return APP_ALREADY_DONE_BEFORE;
}
m_app_result mSingleListInsert(const sMySingleLinkList * s_sll,uint16_t node_number,uint16_t infor_number)
{
	if(s_sll == X_Null) {return APP_POINTER_NULL;}
}
m_app_result mSingleListPullAway(const sMySingleLinkList * s_sll,uint16_t node_number)
{
	if(s_sll == X_Null) {return APP_POINTER_NULL;}
}
X_Boolean mSingleListSizeGet(const sMySingleLinkList * s_sll,uint16_t *p_size)
{
	if(s_sll == X_Null || p_size == X_Null) {return X_False;}
	*p_size = s_sll ->p_param ->used_node_num ;
	return X_True;
}
X_Boolean DoesMySingleListEmpty(const sMySingleLinkList * s_sll)
{
	if(s_sll == X_Null) {return X_False;}
	return (s_sll ->p_param ->used_node_num == 0);
}
X_Void mSingleListClear(const sMySingleLinkList * s_sll)
{
	mSingleListInit(s_sll);
}

X_Boolean mSingleListFindByValue(const sMySingleLinkList * s_sll,uint16_t infor_number,uint16_t *p_node_number)
{
	uint16_t i,addr;
	X_Boolean isFind;
	if(s_sll == X_Null || p_node_number == X_Null) {return X_False;}
	if(s_sll->p_param ->used_node_num == 0)	{return X_False;}

	isFind = X_False;
	addr = s_sll ->p_param ->first_node_address;
	for(i=0;i<s_sll->p_param ->used_node_num;i++)
	{
		if(addr >= s_sll ->ValidNodeNumber) {break;}
		if(s_sll ->p_inf_buf[addr].information_number == infor_number)
		{
			*p_node_number = addr;
			isFind = X_True;
			break;
		}
		addr = s_sll -> p_inf_buf[addr].next_node_address;
	}
	return isFind;
}
X_Boolean mSingleListInformationGetByNodeNumber(const sMySingleLinkList * s_sll,uint16_t node_number,uint16_t *p_infor_num)
{
	uint16_t i,addr;
	X_Boolean isOK;
	if(s_sll == X_Null || p_infor_num == X_Null) {return X_False;}
	if(s_sll->p_param ->used_node_num == 0)	{return X_False;}
	if(node_number > s_sll->p_param ->used_node_num) {return X_False;}

	isOK = X_True;
	addr = s_sll ->p_param ->first_node_address;
	for(i=0;i<node_number;i++)
	{
		if(addr >= s_sll ->ValidNodeNumber) {isOK = X_False;break;}
		addr = s_sll -> p_inf_buf[addr].next_node_address;
	}
	*p_infor_num = s_sll ->p_inf_buf[addr].information_number;
	return isOK;
}
X_Boolean mSingleListUpdataValueByNodeNumber(const sMySingleLinkList * s_sll,uint16_t node_number,uint16_t new_infor_number)
{
	uint16_t i,addr;
	X_Boolean isOK;
	if(s_sll == X_Null ) {return X_False;}
	if(s_sll->p_param ->used_node_num == 0)	{return X_False;}
	if(node_number > s_sll->p_param ->used_node_num) {return X_False;}

	isOK = X_True;
	addr = s_sll ->p_param ->first_node_address;
	for(i=0;i<node_number;i++)
	{
		if(addr >= s_sll ->ValidNodeNumber) {isOK = X_False;break;}
		addr = s_sll -> p_inf_buf[addr].next_node_address;
	}
    s_sll ->p_inf_buf[addr].information_number = new_infor_number;
	return isOK;
}
