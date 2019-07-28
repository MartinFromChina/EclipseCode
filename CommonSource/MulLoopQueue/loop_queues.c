#include "loop_queues.h"

#define BUF_FREE   0
#define BUF_USED   1

typedef enum
{
	QueueEmpty = 0,
	QueueNormal,
	QueueFull,
	QueueStateError,
}QueueState;

static X_Void UpdataListState( const sListManager *p_manager)
{
	if(p_manager == X_Null) {return;}
	if(p_manager->p_LMP->used_node_num == 0)
	{
		p_manager->p_LMP->state = QueueEmpty;
		return;
	}
	if(p_manager->p_LMP->used_node_num >= p_manager->ValidNodeNumber)
	{
		p_manager->p_LMP->state = QueueFull;
		return;
	}
	p_manager->p_LMP->state = QueueNormal;
}

static uint16_t NodeMoveForward(uint16_t max_node,uint16_t current_node)
{
	uint16_t next_node;
	if((current_node+1) >= max_node)
	{
		next_node = 0;
	}
	else
	{
		next_node = current_node + 1;
	}
	return next_node;
}
static uint16_t NodeNumberInMoveForward(const sListManager *p_manager)
{
		uint16_t new_in_node_number;
		if(p_manager == X_Null) {return 0;}
		new_in_node_number = NodeMoveForward(p_manager ->ValidNodeNumber,p_manager->p_LMP->first_in_node_num);
		p_manager->p_LMP->first_in_node_num = new_in_node_number;
		return new_in_node_number;
}
static uint16_t NodeNumberOutMoveForward(const sListManager *p_manager)
{
	uint16_t new_out_node_number;
	if(p_manager == X_Null) {return 0;}
	new_out_node_number = NodeMoveForward(p_manager ->ValidNodeNumber,p_manager->p_LMP->first_out_node_num);
	p_manager->p_LMP->first_out_node_num = new_out_node_number;
	return new_out_node_number;

}
X_Void 		SimpleQueueInitialize(const sListManager *p_manager)
{
	uint16_t i;

	if(p_manager == X_Null) {return;}

	p_manager->p_LMP->state = QueueEmpty;
	p_manager->p_LMP->first_in_node_num = 0;
	p_manager->p_LMP->first_out_node_num = 0;
	p_manager->p_LMP->used_node_num = 0;

	if(p_manager->ValidNodeNumber >= 0xffff) {return;}
	for(i = 0;i< p_manager->ValidNodeNumber;i++)
	{
		p_manager->p_buf[i] = BUF_FREE;
	}

}
uint16_t    SimpleQueueFirstIn(const sListManager *p_manager,X_Boolean *isOK,X_Boolean is_OccupyPermit)
{
	uint16_t buf_number,current_free_node_number;
	if(p_manager == X_Null) {return 0;}

	buf_number = 0;
	current_free_node_number = p_manager->p_LMP->first_in_node_num;

	switch(p_manager->p_LMP->state)
	{
		case QueueEmpty:
		case QueueNormal:
			if(p_manager->p_buf[current_free_node_number] == BUF_FREE )
			{
				NodeNumberInMoveForward(p_manager);
				if(is_OccupyPermit == X_False) {p_manager->p_buf[current_free_node_number] = BUF_USED;}
				else {p_manager->p_buf[current_free_node_number] = BUF_FREE;}
				p_manager->p_LMP->used_node_num ++;
				UpdataListState(p_manager);

				buf_number = current_free_node_number;
				*isOK = X_True;
			}
			else
			{
				*isOK = X_False;
				buf_number = 0;
			}
		break;
		case QueueFull:

			if(p_manager->p_buf[current_free_node_number] == BUF_FREE )
			{
				NodeNumberInMoveForward(p_manager);
				NodeNumberOutMoveForward(p_manager);
				if(is_OccupyPermit == X_False) {p_manager->p_buf[current_free_node_number] = BUF_USED;}
				else {p_manager->p_buf[current_free_node_number] = BUF_FREE;}

				buf_number = current_free_node_number;
				*isOK = X_True;
			}
			else
			{
				*isOK = X_False;
				buf_number = 0;
			}
		break;
		default:
			*isOK = X_False;
			buf_number = 0;
		break;
	}
	return buf_number;
}
uint16_t    SimpleQueueFirstOut(const sListManager *p_manager,X_Boolean *isOK)
{
	uint16_t buf_number,current_filled_node_number;
	if(p_manager == X_Null) {return 0;}

	buf_number = 0;
	current_filled_node_number = p_manager->p_LMP->first_out_node_num;
	switch(p_manager->p_LMP->state)
	{
		case QueueEmpty:
			*isOK = X_False;
			buf_number = 0;
		break;
		case QueueNormal:
		case QueueFull:
			NodeNumberOutMoveForward(p_manager);
			if(p_manager->p_LMP->used_node_num > 0) {p_manager->p_LMP->used_node_num --;}
			UpdataListState(p_manager);

			*isOK = X_True;
			buf_number = current_filled_node_number;
		break;
		default:
			*isOK = X_False;
			buf_number = 0;
		break;
	}
	return buf_number;
}
X_Void      ClearSimpleQueue(const sListManager *p_manager)
{
	SimpleQueueInitialize(p_manager);
}
X_Void      RealseSimpleQueueBuf(const sListManager *p_manager,uint8_t buf_num)
{
	if(p_manager == X_Null) {return;}
	if(buf_num >= p_manager->ValidNodeNumber) {return;}
	p_manager->p_buf[buf_num] = BUF_FREE;
}
uint16_t GetSimpleQueueUsedNodeNumber(const sListManager *p_manager)
{
	if(p_manager == X_Null) {return 0;}
	return p_manager->p_LMP->used_node_num;
}

