#include <stdio.h>
#include "mul_loop_queues.h"

#define PUSH_EQUAL_POP     0

#define MAX_QUEUE_LENGTH   10

APP_LOOPQUEUE_DEF(Queue,Qmanager,Qlength,MAX_QUEUE_LENGTH);

static void NormalDelay(unsigned int counter)
{
	for(; counter > 0; counter --)
	{

	}
}

int main(void)
{
	X_Boolean isOK;
	uint8_t buf_number,i;
	uint8_t data_buf[MAX_QUEUE_LENGTH];

	setbuf(stdout,NULL);
	queueInitialize(Queue,Qmanager,Qlength);
	i = 0;
	while(1)
	{
		NormalDelay(0x2fffffff);
		buf_number = QueueFirstIn(Qmanager,&isOK,X_True);
		if(isOK == X_True)
		{
			data_buf[buf_number] = i;
			printf("buf_number[%d] pushdata %d  ; occupy permit\r\n",buf_number,i);
		}
		else
		{
			printf("push failed  i = %d\r\n",i);
		}
		i ++;

		#if (PUSH_EQUAL_POP == 0)
		buf_number = QueueFirstIn(Qmanager,&isOK,X_False);
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

		buf_number = QueueFirstOut(Qmanager,&isOK);
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
