#include "CharStringDebugModule.h"
//#include <string.h>
#include <stdarg.h>
#include "..\MulLoopQueue\mul_loop_queues.h"

APP_LOOPQUEUE_DEF(string_queue,string_queue_manager,string_queue_length,STRING_DEBUG_BUF_NUMBER);


#if (USE_STRING_DEBUG == 1)

	static char CharBuf[STRING_DEBUG_BUF_NUMBER][MAX_STRING_LENGTH];
	char *StringErrorTranslate(uint32_t error_code,char*(*error_get)(uint32_t error,char* p_cannot))
	{
		#if (USE_ERROR_TRANSLATE == 1)
			return error_get(error_code,X_Null);
		#endif
		#if (USE_ERROR_TRANSLATE == 0)
			return &Canot_Translate[1][0];
		#endif
	}


	X_Boolean OnceEntryCheck(uint16_t *backup_entry,uint16_t new_value) {
		if(*backup_entry != new_value)
		{
			*backup_entry= new_value;
			return X_True;
		}
		return X_False;
	}

	X_Boolean CounterEntryCheck(uint32_t *backup_entry,uint32_t counter_limit){
		if(counter_limit < 1){return X_False;}

		if(*backup_entry == 0)
		{
			*backup_entry = 1;
			return X_True;
		}

		if(*backup_entry < (counter_limit - 1)) {(*backup_entry) ++ ;}
		else {*backup_entry = 0 ;}
		return X_False;
	}
	static void DebugPopString(char *string)
	{
		DEBUG_POP_METHOD(string);
	}


	X_Boolean PushStringIntoCharBuf(uint8_t length,char *string, ...)
	{
		va_list ParamList;
		char *p_buf;
		uint8_t bufnumber;
		X_Boolean isOK;

		if(length == 0) {return X_False;}
		if(length > (MAX_STRING_LENGTH - MAX_STRING_ARG_NUMBER)) {return X_False;}

		#if(USE_OPERATION_SYSTEM == 1)
		GET_DEBUG_SEMPHORE_METHOD;
		#endif
		DEBUG_ENTER_CRITICAL_METHOD;

		bufnumber = QueueFirstIn(string_queue_manager,&isOK,X_False);
		if(isOK != X_True)
		{
			DEBUG_EXIT_CRITICAL_METHOD;
			#if(USE_OPERATION_SYSTEM == 1)
			RELEASE_DEBUG_SEMPHORE_METHOD;
			#endif
			return X_False;
		}

		p_buf = &CharBuf[bufnumber][0];
		va_start(ParamList, string);
		vsprintf(p_buf, string, ParamList);
		va_end(ParamList);

		p_buf[length-1] = '\0';

		DEBUG_EXIT_CRITICAL_METHOD;
		#if(USE_OPERATION_SYSTEM == 1)
		RELEASE_DEBUG_SEMPHORE_METHOD;
		#endif

		return X_True;

	}
	void StringDebugFlush(void)
	{
		X_Boolean isOK;
		uint8_t bufnumber;
		char *p_buf;

		if(DoesQueueEmpty() == X_True){return;}

		#if(USE_OPERATION_SYSTEM == 1)
		GET_DEBUG_SEMPHORE_METHOD;
		#endif
		DEBUG_ENTER_CRITICAL_METHOD;
		bufnumber = QueueFirstOut(string_queue_manager,&isOK);

		while(isOK == X_True)
		{
			p_buf = &CharBuf[bufnumber][0];
			DebugPopString(p_buf);
			bufnumber = QueueFirstOut(string_queue_manager,&isOK);
		}

		DEBUG_EXIT_CRITICAL_METHOD;
		#if(USE_OPERATION_SYSTEM == 1)
		RELEASE_DEBUG_SEMPHORE_METHOD;
		#endif
	}
	void StringDebugProcess(void)
	{
		X_Boolean isOK;
		uint8_t bufnumber;
		char *p_buf;

		if(DoesQueueEmpty() == X_True)
		{
//			printf("empty\r\n");
			return;
		}

//		printf("not empty\r\n");

		#if(USE_OPERATION_SYSTEM == 1)
		GET_DEBUG_SEMPHORE_METHOD;
		#endif
		DEBUG_ENTER_CRITICAL_METHOD;
		bufnumber = QueueFirstOut(string_queue_manager,&isOK);

		if(isOK == X_True)
		{
			p_buf = &CharBuf[bufnumber][0];
			DebugPopString(p_buf);
		}

		DEBUG_EXIT_CRITICAL_METHOD;
		#if(USE_OPERATION_SYSTEM == 1)
		RELEASE_DEBUG_SEMPHORE_METHOD;
		#endif
	}
#else
	static char Canot_Translate[2][9] = {
			"unknow",
			"unsupport",
	};
	char *StringErrorTranslate(uint32_t error_code,char*(*error_get)(uint32_t error))
	{
		return &Canot_Translate[1][0];
	}
	X_Boolean PushStringIntoCharBuf(uint8_t length,char *string, ...)
	{
		return X_False;
	}

	void StringDebugFlush(void)
	{

	}
	void StringDebugProcess(void)
	{

	}
#endif



void StringDebugInit(void(*p_init)(void))
{
	queueInitialize(string_queue,string_queue_manager,string_queue_length);
	(*p_init)();
	String_Debug(FOW_NOW_DEBUG,(30,"hello %s \r\n","I am string debug"));
}
