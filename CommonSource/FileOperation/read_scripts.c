#include "read_scripts.h"
#include "..\CharStringDebug\CharStringDebugModule.h"

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
#define MaxCommandLength     60

static FILE* pFile;// just a pointer ? why?
static char CmdLine[MaxCommandReadTimes][MaxCommandLength];
static uint32_t commandLineNum,MaxcommandLineNum;

/*
static X_Boolean GetCommandInfoFromCmdLine(CommandParam *p_commparam,X_Boolean (*command_analysis)(char * command_string,CommandParam *p_commparam))
{
	X_Boolean isCommandAnalysisSuccessed;
	if((commandLineNum+1 >= MaxCommandReadTimes) || (commandLineNum+1 >= MaxcommandLineNum ) )
	{
		p_commparam->command = NoCommandAnyMore;
		return X_False;
	}

	isCommandAnalysisSuccessed = command_analysis(&CmdLine[commandLineNum][0],p_commparam);
	commandLineNum ++ ;
	return isCommandAnalysisSuccessed;
}
*/

/*
X_Boolean GetCommandFromOneTxtLine(CommandParam *p_commparam)
{
	X_Boolean isNewComamndCome;
	Command   comm;

	isNewComamndCome = X_False;
	comm = UnknowCommand;
	if(isOpenFileSucessed == X_False || isCommandReadFinished == X_False) {return isNewComamndCome;}
	switch(rs)
	{
		case StateIdle:
		case StateWait:
		case StateStop:
			break;
		case StateRead:
			isNewComamndCome = GetCommandInfoFromCmdLine(p_commparam,CommandAnalysis);
		    comm = p_commparam->command;
			break;
		default:
			break;
	}

	switch(rs)
	{
		case StateIdle:
			PRINTF_Debug_Once(READ_SCRIPTS_STATE_DEBUG,ONCE_ENTRY_CONVER("scrp")
					,StateIdle,(" --StateIdle\r\n"));
			rs = StateRead;
			break;
		case StateRead:
			PRINTF_Debug_Once(READ_SCRIPTS_STATE_DEBUG,ONCE_ENTRY_CONVER("scrp")
					,StateRead,(" --StateRead\r\n"));
			if(comm == Wait )//command == Wait
			{
				wait_time_counter = 0;
				rs = StateWait;
			}
			else if(comm == UnknowCommand)//command == nothing
			{
				rs = StateIdle;
			}
			else if(comm == NoCommandAnyMore)
			{
				rs = StateStop;
			}
			break;
		case StateWait:
			PRINTF_Debug_Once(READ_SCRIPTS_STATE_DEBUG,ONCE_ENTRY_CONVER("scrp")
					,StateWait,(" --StateWait\r\n"));
			wait_time_counter ++;
			if(wait_time_counter >= p_commparam->wait_time)// wait timeout
			{
				rs = StateIdle;
			}
			break;
		case StateStop:
			PRINTF_Debug_Once(READ_SCRIPTS_STATE_DEBUG,ONCE_ENTRY_CONVER("scrp")
								,StateStop,(" --StateStop\r\n"));
			break;
		default:
			PRINTF_Debug_Once(READ_SCRIPTS_STATE_DEBUG,ONCE_ENTRY_CONVER("scrp")
					,StateWait+1,(" --default\r\n"));
			rs = StateIdle;
			break;
	}

	return isNewComamndCome;

}
*/
void ReadScriptsInit(FILE* (*open_file)(void))
{
	isOpenFileSucessed = X_False;
	isCommandReadFinished = X_False;
	//pFile = fopen(".//test_target//txt_script//command_temp.txt", "r");
//	pFile = fopen(".//test_target//txt_script//command_temp_usart.txt", "r");
	pFile = (*open_file)();
	if(pFile != X_Null)
	{
		isOpenFileSucessed = X_True;
	}
	if(isOpenFileSucessed == X_True)
	{
		commandLineNum = 0;
		while (!feof(pFile))
		{
			if(fgets(&CmdLine[commandLineNum][0],MaxCommandLength,pFile) != X_Null)
			{
				MaxcommandLineNum = commandLineNum + 1;
				String_Debug(SCRIPTS_COMMAND_CONTEXT_DEBUG,(30,"%s\r\n",&CmdLine[commandLineNum][0]));
				if((commandLineNum+1 >= MaxCommandReadTimes)
								||	(CmdLine[commandLineNum][0] ==':'
									&& CmdLine[commandLineNum][1] == 'e'
									&& CmdLine[commandLineNum][2] == 'n'
									&& CmdLine[commandLineNum][3] == 'd'))
					{
					String_Debug(SCRIPTS_COMMAND_CONTEXT_DEBUG,(30," read command finished\r\n"));
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

