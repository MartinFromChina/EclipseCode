#include "command.h"
#include "..\CharStringDebug\CharStringDebugModule.h"
#include "..\MulLoopQueue\mul_loop_queues.h"

APP_LOOPQUEUE_DEF(command_queue,command_queue_manager,command_queue_length,MaxCommandBufNumber);

static const uint8_t CharToHexTable[MaxCharToHexTableSize]={
	   0,	   1,	   2,	   3,      4,      5,      6,      7,      8,      9,// 48~57
	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	 0xA,	 0xB,	 0xC,//58~67
	 0xD,	 0xE, 	 0xF,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,//68~77
	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,//78~87
	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff, 	 0xa,//88~97
	 0xb,	 0xc,	 0xd, 	 0xe, 	 0xf//98~102
};

static uint8_t CommandBuf[MaxCommandBufNumber][MaxCommandLength];

void CommandReceivedInit(void)
{
	queueInitialize(command_queue,command_queue_manager,command_queue_length);
}
static uint8_t GetStringLength(char *p_strings)
{
	uint8_t i;
	for(i=0;i<255;i++)
	{
		if(p_strings[i]=='\0' )
		{
			break;
		}
	}
	if(i<1){i=1;}
	return (i-1);
}
static uint8_t CharToHex(char hexchar_H,char hexchar_L,X_Boolean *isOK)
{
	uint8_t result;
	if(hexchar_H < 48 || hexchar_H >102 ||hexchar_L < 48 || hexchar_L >102)
	{
		*isOK = X_False;
		return 0;
	}
	hexchar_H = hexchar_H - 48;
	hexchar_L = hexchar_L - 48;
	if(CharToHexTable[(uint8_t)hexchar_H] == 0xff || CharToHexTable[(uint8_t)hexchar_L] == 0xff)
	{
		*isOK = X_False;
		return 0;
	}
	result = CharToHexTable[(uint8_t)hexchar_L] + (CharToHexTable[(uint8_t)hexchar_H]<<4);
	*isOK = X_True;
	return result;
}
static X_Boolean CopyHexModeCommandByCharType(char *p_command)
{
	X_Boolean isOK;
	uint16_t bufnumber,i,commandsize;
	bufnumber = QueueFirstIn(command_queue_manager,&isOK,X_False);
	if(isOK == X_True && bufnumber < MaxCommandBufNumber)
	{
		commandsize = GetStringLength(p_command);
		if(commandsize < CommandStartAddr) {return X_False;}
		commandsize = commandsize - CommandStartAddr;
		if(commandsize %2 != 0) {commandsize = commandsize + 1;}
		commandsize = commandsize /2;
		if(commandsize > MaxCommandLength){commandsize = MaxCommandLength;}

		for(i=0;i<commandsize;i++)
		{
			CommandBuf[bufnumber][i] = CharToHex(p_command[CommandStartAddr+(i*2)],p_command[CommandStartAddr+(i*2)+1],&isOK);
			String_Debug(SCRIPTS_COMMAND_CONTEXT_DEBUG,(10," %2x\r\n",CommandBuf[bufnumber][i]));
		}
		return X_True;
	}
	return X_False;
}

static X_Boolean CopyHexModeCommandByCharTypeImmediately(char *p_command)
{
	X_Boolean isOK;
	uint16_t bufnumber,i,commandsize;

	bufnumber = 0;
	commandsize = GetStringLength(p_command);
	if(commandsize < CommandStartAddr) {return X_False;}
	commandsize = commandsize - CommandStartAddr;
	if(commandsize %2 != 0) {commandsize = commandsize + 1;}
	commandsize = commandsize /2;
	if(commandsize > MaxCommandLength){commandsize = MaxCommandLength;}

	for(i=0;i<commandsize;i++)
	{
		CommandBuf[bufnumber][i] = CharToHex(p_command[CommandStartAddr+(i*2)],p_command[CommandStartAddr+(i*2)+1],&isOK);
		String_Debug(SCRIPTS_COMMAND_CONTEXT_DEBUG,(10," %2x\r\n",CommandBuf[bufnumber][i]));
	}
	return X_True;
}

X_Boolean CommandAnalysis(char * command_string,ScriptCommandParam *p_commparam,X_Boolean isImmediately)
{
	X_Boolean isOk;
	uint8_t result;
	char str_1[] = ":wait:10";
	char str_2[] = ":command:10";
	p_commparam->command = UnknowCommand;
	if(strncmp(str_1,command_string,6) == 0)
	{
		p_commparam->command = Wait;

		result = CharToHex(command_string[6],command_string[7],&isOk);
		if( isOk == X_True)
		{
			p_commparam->wait_time = result;
			result = CharToHex(command_string[8],command_string[9],&isOk);
			if(isOk == X_True)
			{
				p_commparam->wait_time = result + (uint16_t)((p_commparam->wait_time)<<8);
				String_Debug(SCRIPTS_COMMAND_CONTEXT_DEBUG,
						(25," wait_time :%2x\r\n",p_commparam->wait_time));
				return X_True;
			}
			return X_True;
		}

		p_commparam->wait_time = 100;// 100 is a random number
		return X_True;
	}
	else if(strncmp(str_2,command_string,9) == 0)
	{
		p_commparam->command = KnowCommand;  //
		if(isImmediately == X_False)
		{
			return CopyHexModeCommandByCharType(command_string);
		}
		else
		{
			return CopyHexModeCommandByCharTypeImmediately(command_string);
		}
	}
	else
	{
		return X_False;
	}

}

X_Boolean LoadCommand(uint8_t **p_command_value,uint8_t *length,X_Boolean isImmediately)
{
	X_Boolean isOK;
	uint16_t bufnumber;

	if(isImmediately == X_True)
	{
		bufnumber = 0;
		if(CommandBuf[bufnumber][CommandLengthAddr] > MaxCommandLength ) {*length = MaxCommandLength;}
		else {*length = CommandBuf[bufnumber][CommandLengthAddr];}

		*p_command_value = &CommandBuf[bufnumber][0];
		return X_True;
	}

	bufnumber = QueueFirstOut(command_queue_manager,&isOK);
	if(isOK == X_False) {bufnumber = 0;}

	if(CommandBuf[bufnumber][CommandLengthAddr] > MaxCommandLength ) {*length = MaxCommandLength;}
	else {*length = CommandBuf[bufnumber][CommandLengthAddr];}

	*p_command_value = &CommandBuf[bufnumber][0];
	return isOK;
}



