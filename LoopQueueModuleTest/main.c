#include <stdio.h>
//#include "mul_loop_queues.h"
#include "..\CommonSource\MulLoopQueue\loop_queues.h"

#define PUSH_EQUAL_POP     0

#define MAX_QUEUE_LENGTH   10
#define MAX_LONG_QUEUE_LENGTH   270

#define CALL_CONDITON_WITHOUT_RESERT    280
#define CALL_CONDITON_RESTART			300
#define CALL_CONDITON_WITH_RESTART       350

SIMPLE_LOOPQUEUE_DEF(p_queue_manager,MAX_QUEUE_LENGTH);
SIMPLE_LOOPQUEUE_DEF(p_long_queue_manager,270);

static void NormalDelay(unsigned int counter)
{
	for(; counter > 0; counter --)
	{

	}
}
static void PushPopSimple(void);
static void PushPopTest(void);
static void PushTest(X_Boolean isOccupyPermit);
static void PopTest(void);
static  uint16_t i;
static X_Boolean isStop = X_False;
int main(void)
{
	uint16_t counter;
	setbuf(stdout,NULL);
	SimpleQueueInitialize(p_queue_manager);
	i = 0;
	counter = 0;
	isStop = X_False;
	while(1)
	{
		PushPopSimple();
//		PushPopTest();
//		if(counter < CALL_CONDITON_WITH_RESTART)
//		{
//			counter ++;
//			PushTest(X_False);
//		}
//		else
//		{
//			PopTest();
//		}
	}
}

static  uint16_t buf_number;
static 	uint8_t data_buf[MAX_LONG_QUEUE_LENGTH];

static void PushPopSimple(void)
{
	X_Boolean isOK;
	NormalDelay(0x1fffffff);
	buf_number = SimpleQueueFirstIn(p_queue_manager,&isOK,X_False);
	if(isOK == X_True)
	{
		data_buf[buf_number] = i;
		printf("buf_number[%d] pushdata %d  ; occupy not permit\r\n",buf_number,i);
	}
	else
	{
		printf("push failed  i = %d\r\n",i);
	}
	i++;
	buf_number = SimpleQueueFirstOut(p_queue_manager,&isOK);
	if(isOK == X_True)
	{
		printf("buf_number[%d] popdata %d \r\n",buf_number,data_buf[buf_number]);
//		RealseSimpleQueueBuf(p_queue_manager,buf_number);
	}
	else
	{
		printf("pop failed \r\n");
	}
}

static void PushPopTest(void)
{
	X_Boolean isOK;
	NormalDelay(0x1fffffff);
	buf_number = SimpleQueueFirstIn(p_queue_manager,&isOK,X_False);
	if(isOK == X_True)
	{
		data_buf[buf_number] = i;
		printf("buf_number[%d] pushdata %d  ; occupy not permit\r\n",buf_number,i);
	}
	else
	{
		printf("push failed  i = %d\r\n",i);
	}
	i ++;

	#if (PUSH_EQUAL_POP == 0)
	buf_number = SimpleQueueFirstIn(p_queue_manager,&isOK,X_False);
	if(isOK == X_True)
	{
		data_buf[buf_number] = i;
		printf("buf_number[%d] pushdata %d  ; occupy not permit\r\n",buf_number,i);
	}
	else
	{
		printf("push failed  i = %d\r\n",i);
	}
	i ++;
	#endif

	buf_number = SimpleQueueFirstOut(p_queue_manager,&isOK);
	if(isOK == X_True)
	{
		printf("buf_number[%d] popdata %d \r\n",buf_number,data_buf[buf_number]);
		RealseSimpleQueueBuf(p_queue_manager,buf_number);
	}
	else
	{
		printf("pop failed \r\n");
	}

}

static void PushTest(X_Boolean isOccupyPermit)
{
	X_Boolean isOK;
	NormalDelay(0x2ffffff);
	buf_number = SimpleQueueFirstIn(p_long_queue_manager,&isOK,isOccupyPermit);
	if(isOK == X_True)
	{
		data_buf[buf_number] = i;
		printf("buf_number[%d] pushdata %d  ; occupy permit %d \r\n",buf_number,i,isOccupyPermit);
		data_buf[buf_number] = i;
	}
	else
	{
		printf("push failed  i = %d\r\n",i);
	}
	i ++;

	if(i == CALL_CONDITON_RESTART)
	{
		ClearSimpleQueue(p_long_queue_manager);
	}
}

static void PopTest(void)
{
	X_Boolean isOK;
	NormalDelay(0x2ffffff);
	if(isStop == X_True) {return;}
	if(GetSimpleQueueUsedNodeNumber(p_long_queue_manager) == 0)
	{
		isStop = X_True;
		printf("queue empty\r\n");
		buf_number = SimpleQueueFirstOut(p_long_queue_manager,&isOK);
		if(isOK == X_True)
		{
			printf("buf_number[%d] popdata %d \r\n",buf_number,data_buf[buf_number]);
		}
		else
		{
			printf("pop failed \r\n");
		}
	}
	else
	{
		buf_number = SimpleQueueFirstOut(p_long_queue_manager,&isOK);
		if(isOK == X_True)
		{
			printf("buf_number[%d] popdata %d \r\n",buf_number,data_buf[buf_number]);
		}
		else
		{
			printf("pop failed \r\n");
		}
	}


}
