#include "read_scripts.h"
#include "command.h"
#include "..\CharStringDebug\CharStringDebugModule.h"


STRING_DEBUG_ONCE_ENTRY_DEF(p_entry,100);


static X_Boolean isOpenFileSucessed,isCommandReadFinished;

typedef enum
{
	StateIdle,
	StateRead,
	StateWait,
	StateStop,
}ReadState;

static uint32_t wait_time_counter;
static ReadState rs;

#define MaxCommandReadTimes  100
#define MaxScriptsCommandLength     60

static FILE* pFile;// just a pointer ? why?
static char CmdLine[MaxCommandReadTimes][MaxScriptsCommandLength];
static uint32_t commandLineNum,MaxcommandLineNum;
static ScriptCommandParam SCP;


static X_Boolean GetCommandInfoFromCmdLine(ScriptCommandParam *p_commparam
				,X_Boolean (*command_analysis)(char * command_string,ScriptCommandParam *p_commparam,X_Boolean isImmediately)
				,X_Boolean isImmediately)
{
	X_Boolean isCommandAnalysisSuccessed;
	if((commandLineNum+1 >= MaxCommandReadTimes) || (commandLineNum+1 >= MaxcommandLineNum ) )
	{
		String_Debug(READ_SCRIPTS_COMMAND_DEBUG,(40,"NoCommandAnyMore at line :%d \r\n",commandLineNum));
		p_commparam->command = NoCommandAnyMore;
		return X_False;
	}

	isCommandAnalysisSuccessed = command_analysis(&CmdLine[commandLineNum][0],p_commparam,isImmediately);
	commandLineNum ++ ;
	return isCommandAnalysisSuccessed;
}

static X_Boolean GetCommandFromOneTxtLine(ScriptCommandParam *p_commparam
					,X_Boolean (*command_analysis)(char * command_string,ScriptCommandParam *p_commparam,X_Boolean isImmediately)
					,X_Boolean isImmediately)
{
	X_Boolean isNewComamndCome;
	ScriptCommandType   comm;

	isNewComamndCome = X_False;
	comm = UnknowCommand;
	if(isOpenFileSucessed == X_False || isCommandReadFinished == X_False) {return isNewComamndCome;}

	switch(rs)
	{
		case StateIdle:
			String_Debug_Once(READ_SCRIPTS_STATE_DEBUG,p_entry,StateIdle,(30," --StateIdle\r\n"));
			rs = StateRead;
			break;
		case StateRead:
			String_Debug_Once(READ_SCRIPTS_STATE_DEBUG,p_entry,StateRead,(30," --StateRead\r\n"));
			isNewComamndCome = GetCommandInfoFromCmdLine(p_commparam,command_analysis,isImmediately);
			comm = p_commparam->command;
			if(comm == Wait )//command == Wait
			{
				isNewComamndCome = X_False;
				wait_time_counter = 0;
				rs = StateWait;
			}
			else if(comm == UnknowCommand)//command == nothing
			{
				isNewComamndCome = X_False;
				rs = StateIdle;
			}
			else if(comm == NoCommandAnyMore)
			{
				isNewComamndCome = X_False;
				rs = StateStop;
			}
			break;
		case StateWait:
			String_Debug_Once(READ_SCRIPTS_STATE_DEBUG,p_entry,StateWait,(30," --StateWait\r\n"));
			wait_time_counter ++;
			if(wait_time_counter >= p_commparam->wait_time)// wait timeout
			{
				rs = StateIdle;
			}
			break;
		case StateStop:
			String_Debug_Once(READ_SCRIPTS_STATE_DEBUG,p_entry,StateStop,(30," --StateStop\r\n"));
			break;
		default:
			String_Debug_Once(READ_SCRIPTS_STATE_DEBUG,p_entry,StateWait+1,(30," --default\r\n"));
			rs = StateIdle;
			break;
	}

	return isNewComamndCome;

}

static uint8_t *p_command; // !!! can't in the function ScriptCommandHandle scope
static uint8_t length;

static uint32_t ReadScriptAndDoAsCommand(uint32_t (*ExecuCommandAndGetNextOne)(uint8_t* p_command,uint8_t length,uint32_t current_command_line,
										 X_Boolean (*CallFunction_Entry)(uint8_t func_num,X_Void * p_param))
										,X_Boolean (*CallFunction_Entry)(uint8_t func_num,X_Void * p_param))
{
	X_Boolean isOK,isNewCommand;
	isNewCommand = GetCommandFromOneTxtLine(&SCP,CommandAnalysis,X_True);
	isOK = LoadCommand(&p_command,&length,X_True);
	if(isOK == X_True && isNewCommand == X_True)
	{
		return ExecuCommandAndGetNextOne(p_command,length,commandLineNum,CallFunction_Entry);
	}
	return commandLineNum;

}
void ReadScriptsInit(FILE* (*open_file)(void))
{
	isOpenFileSucessed = X_False;
	isCommandReadFinished = X_False;

	CommandReceivedInit();

	pFile = (*open_file)();
	if(pFile != X_Null)
	{
		isOpenFileSucessed = X_True;
	}
	else
	{
		String_Debug(SCRIPTS_COMMAND_CONTEXT_DEBUG,(30,"open file failed\r\n"));
	}
	if(isOpenFileSucessed == X_True)
	{
		commandLineNum = 0;
		while (!feof(pFile))
		{
			if(fgets(&CmdLine[commandLineNum][0],MaxScriptsCommandLength,pFile) != X_Null)
			{
				MaxcommandLineNum = commandLineNum + 1;
				String_Debug(SCRIPTS_COMMAND_CONTEXT_DEBUG,(30,"%s\r\n",&CmdLine[commandLineNum][0]));
				if((commandLineNum+1 >= MaxCommandReadTimes)
								||	(CmdLine[commandLineNum][0] ==':'
									&& CmdLine[commandLineNum][1] == 'e'
									&& CmdLine[commandLineNum][2] == 'n'
									&& CmdLine[commandLineNum][3] == 'd'))
					{
					String_Debug(SCRIPTS_COMMAND_CONTEXT_DEBUG,(30,"MaxcommandLineNum : %d\r\n",MaxcommandLineNum));
					isCommandReadFinished = X_True;
					break;
					}
				commandLineNum ++ ;
			}

		}
		fclose(pFile);

	}
	commandLineNum = 0;
	rs	  =  StateIdle;


}

void ScriptCommandHandle(X_Boolean(*doAsCommand)(uint8_t* p_command,uint8_t length))
{
	X_Boolean isOK;

	GetCommandFromOneTxtLine(&SCP,CommandAnalysis,X_False);
	isOK = LoadCommand(&p_command,&length,X_False);
	if(isOK == X_True)
	{
	    doAsCommand(p_command,length);
	}
}

void ConditionalScriptCommandHandle(uint32_t (*ExecuCommandAndGetNextOne)(uint8_t* p_command,uint8_t length,uint32_t current_command_line
											,X_Boolean (*CallFunction)(uint8_t func_num,X_Void * p_param))
									,X_Boolean (*CallFunction_Entry)(uint8_t func_num,X_Void * p_param)
		)
{
	commandLineNum = ReadScriptAndDoAsCommand(ExecuCommandAndGetNextOne,CallFunction_Entry);
//	String_Debug(READ_SCRIPTS_COMMAND_DEBUG,(35,"CurrentcommandLineNum : %d\r\n",commandLineNum));
}
