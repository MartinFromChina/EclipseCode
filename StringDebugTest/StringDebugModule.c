#include "StringDebugModule.h"
#include "LocalQueue.h"

#include <stdarg.h>
#include <string.h>


/*
#ifndef va_copy
# ifdef __va_copy
# define va_copy(DEST,SRC) __va_copy((DEST),(SRC))
# else
# define va_copy(DEST, SRC) memcpy((&DEST), (&SRC), sizeof(va_list))
# endif
#endif
*/


APP_LOOPQUEUE_DEF(string_queue,string_queue_manager,string_queue_length,STRING_DEBUG_BUF_NUMBER);

#if (USE_STRING_DEBUG == 0)
static char Canot_Translate[2][9] = {
		"unknow",
		"unsupport",
};
#endif

#if (USE_STRING_DEBUG == 1)
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


typedef struct
{
	char 		string_buf[MAX_STRING_LENGTH];
	int			arg_buf[MAX_STRING_ARG_NUMBER];
	uint8_t    	arg_number;
}StringParam;

static StringParam string_param_buf[STRING_DEBUG_BUF_NUMBER];

/*
 * can't handle float type data !!!!
 */
static void DebugPrintString(StringParam * p_SP){
		switch(p_SP->arg_number)
		{
		case 0:
			DEBUG_POP_METHOD_0(p_SP->string_buf);
			break;
		case 1:
			DEBUG_POP_METHOD_1(p_SP->string_buf,p_SP->arg_buf[0]);
			break;
		case 2:
			DEBUG_POP_METHOD_2(p_SP->string_buf
					,p_SP->arg_buf[0]
					,p_SP->arg_buf[1]);
			break;
		case 3:
			DEBUG_POP_METHOD_3(p_SP->string_buf
					,p_SP->arg_buf[0]
				  ,p_SP->arg_buf[1]
					,p_SP->arg_buf[2]);
			break;
		case 4:
			DEBUG_POP_METHOD_4(p_SP->string_buf
					,p_SP->arg_buf[0]
					,p_SP->arg_buf[1]
					,p_SP->arg_buf[2]
					,p_SP->arg_buf[3]);
			break;
		case 5:
			DEBUG_POP_METHOD_5(p_SP->string_buf
					,p_SP->arg_buf[0]
					,p_SP->arg_buf[1]
					,p_SP->arg_buf[2]
					,p_SP->arg_buf[3]
					,p_SP->arg_buf[4]);
			break;
		case 6:
			DEBUG_POP_METHOD_6(p_SP->string_buf
					,p_SP->arg_buf[0]
					,p_SP->arg_buf[1]
					,p_SP->arg_buf[2]
					,p_SP->arg_buf[3]
					,p_SP->arg_buf[4]
					,p_SP->arg_buf[5]);
			break;
		case 7:
			DEBUG_POP_METHOD_7(p_SP->string_buf
					,p_SP->arg_buf[0]
					,p_SP->arg_buf[1]
					,p_SP->arg_buf[2]
					,p_SP->arg_buf[3]
					,p_SP->arg_buf[4]
					,p_SP->arg_buf[5]
					,p_SP->arg_buf[6]);
			break;
		default:
			//error
			DEBUG_POP_METHOD_1("arg_number :%d is beyond the limit \r\n",p_SP->arg_number);
			break;
		}
}
static void StringDebugEnterCriticalRegion(void)
{
	DEBUG_ENTER_CRITICAL_METHOD	;
}
static void StringDebugExitCriticalRegion(void)
{
	DEBUG_EXIT_CRITICAL_METHOD	;
}
void DebugPushString(uint8_t param_num,char *string, ...)
{
	va_list ParamList;
	X_Boolean isOK;
	uint8_t bufnumber,i;
	StringParam* pSP;

	if(strlen(string)  > MAX_STRING_LENGTH) {return;}
	if(param_num > MAX_STRING_ARG_NUMBER){return;}
	
	#if(USE_OPERATION_SYSTEM == 1)
	GET_DEBUG_SEMPHORE_METHOD;
	#endif
	StringDebugEnterCriticalRegion();

	bufnumber = QueueFirstIn(string_queue_manager,&isOK,X_False);
	if(isOK != X_True) {return;}
	pSP = &string_param_buf[bufnumber];
	

	strcpy(pSP->string_buf,string);

	StringDebugExitCriticalRegion();
	#if(USE_OPERATION_SYSTEM == 1)
	RELEASE_DEBUG_SEMPHORE_METHOD;
	#endif

	/****************
	 *need a check for the real number of param
	 */
	va_start(ParamList, string);
	for(i=0;i<param_num;i++)
	{
		pSP->arg_buf[i] = va_arg(ParamList,int);
	}
	pSP->arg_number = param_num;
	va_end(ParamList);
}
void StringDebugFlush(void)
{
	X_Boolean isOK;
	uint8_t bufnumber;
	StringParam* pSP;

	bufnumber = QueueFirstOut(string_queue_manager,&isOK);
	
	while(isOK == X_True)
	{
		pSP = &string_param_buf[bufnumber];
		DebugPrintString(pSP);
		bufnumber = QueueFirstOut(string_queue_manager,&isOK);
	}
}
void StringDebugProcess(void)
{
	X_Boolean isOK;
	uint8_t bufnumber;
	StringParam* pSP;

	bufnumber = QueueFirstOut(string_queue_manager,&isOK);

	if(isOK == X_True)
	{
		pSP = &string_param_buf[bufnumber];
		DebugPrintString(pSP);
	}
}

#else

char *StringErrorTranslate(uint32_t error_code,char*(*error_get)(uint32_t error))
{
	return &Canot_Translate[1][0];
}


void StringDebugFlush(void)
{

}

#endif

static void DebugPopStringMethodInit(void(*p_init)(void))
{
	(*p_init)();
}

void StringDebugInit(void(*p_init)(void))
{
	queueInitialize(string_queue,string_queue_manager,string_queue_length);
	DebugPopStringMethodInit(p_init);
	String_Debug(FOW_NOW_DEBUG,(1,"hello %s \r\n","I am string debug"));
}
