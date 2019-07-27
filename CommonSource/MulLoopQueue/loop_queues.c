#include "loop_queues.h"
#include "..\Math\bit_operation.h"

#define GetBitMethod  	byte_getbit
#define SetBitMethod  	byte_setbit
#define ClearBitMethod  byte_clearbit
#define FactorSetMethod ByteFactorSet

#define isBufFreeBitPosition     		0
#define isOccupyPermitBitPosition       1

typedef enum
{
	QueueEmpty = 0,
	QueueNormal,
	QueueFull,
	QueueStateError,
}QueueState;

static QueueState GetQueueState(const sListManager *p_manager)
{
	if(p_manager == X_Null) {return QueueStateError;}
	if((p_manager->p_LMP->state) >= QueueStateError) {return QueueStateError;}
	return (p_manager->p_LMP->state);
}
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
static X_Boolean DoesNodeOccupyPermit(const sListManager *p_manager,uint16_t node_num)
{
	if(p_manager == X_Null) {return X_False;}
	if(node_num >= p_manager->ValidNodeNumber) {return X_False;}
	return (GetBitMethod(p_manager->p_buf[node_num],isOccupyPermitBitPosition) == 0);
}
static X_Void SetNodeFreeOrNot(const sListManager *p_manager,uint16_t node_num,X_Boolean isFree)
{
	if(p_manager == X_Null) {return ;}
	if(node_num >= p_manager->ValidNodeNumber) {return ;}

	p_manager->p_buf[node_num] = FactorSetMethod(p_manager->p_buf[node_num]
												 ,isBufFreeBitPosition
												 ,(isFree == X_True)? Clear:Set);

}
static X_Void SetNodePermitOccupyOrNot(const sListManager *p_manager,uint16_t node_num,X_Boolean isOccupyPermit)
{
	if(p_manager == X_Null) {return ;}
	if(node_num >= p_manager->ValidNodeNumber) {return ;}

	p_manager->p_buf[node_num] = FactorSetMethod(p_manager->p_buf[node_num]
												 ,isOccupyPermitBitPosition
												 ,(isOccupyPermit == X_True)? Clear:Set);

}
static uint16_t NodeMoveForward(uint16_t max_node,uint16_t current_node)
{
	uint16_t next_node;
	if(current_node >= max_node)
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
		p_manager->p_buf[i] = 0;
	}

}
uint16_t    SimpleQueueFirstIn(const sListManager *p_manager,X_Boolean *isOK,X_Boolean is_OccupyPermit)
{
	uint16_t buf_number,current_free_node_number;
	if(p_manager == X_Null) {return 0;}
	UpdataListState(p_manager);

	buf_number = 0;
	current_free_node_number = p_manager->p_LMP->first_in_node_num;
	switch(p_manager->p_LMP->state)
	{
		case QueueEmpty:
		case QueueNormal:
			NodeNumberInMoveForward(p_manager);
			SetNodeFreeOrNot(p_manager,current_free_node_number,X_False);
			SetNodePermitOccupyOrNot(p_manager,current_free_node_number,is_OccupyPermit);
			p_manager->p_LMP->used_node_num ++;
			UpdataListState(p_manager);

			buf_number = current_free_node_number;
			*isOK = X_True;
		break;
		case QueueFull:

			if(DoesNodeOccupyPermit(p_manager,current_free_node_number) == X_True)
			{
				NodeNumberInMoveForward(p_manager);
				NodeNumberOutMoveForward(p_manager);
				SetNodeFreeOrNot(p_manager,current_free_node_number,X_False);
				SetNodePermitOccupyOrNot(p_manager,current_free_node_number,is_OccupyPermit);

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

	UpdataListState(p_manager);

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
			//
			NodeNumberOutMoveForward(p_manager);
			p_manager->p_buf[current_filled_node_number] = 0; // set node free and OccupyPermit
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
X_Boolean   DoesSimpleQueueEmpty(const sListManager *p_manager)
{
	return (GetQueueState(p_manager) == QueueEmpty);
}

