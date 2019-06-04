#include "user_log.h"
#include <time.h>
#include <sys/timeb.h>
#include <stdarg.h>
#include "..\IrqAndTimer\user_irq.h"

#define MAXLOGSIZE 20000000
#define ARRSIZE(x) (sizeof(x)/sizeof(x[0]))

char logstr[16000];
char datestr[16];
char timestr[16];
char mss[4];

FILE *flog;

static char logfilename1[]="MyLog1.log";
static char logfilename2[]="MyLog2.log";

static void LogV(const char *pszFmt,va_list argp) {
    struct tm *now;
    struct timeb tb;


    if (NULL==pszFmt||0==pszFmt[0]) return;
    if (-1==_vsnprintf(logstr,ARRSIZE(logstr),pszFmt,argp)) logstr[ARRSIZE(logstr)-1]=0;
    ftime(&tb);
    now=localtime(&tb.time);
    sprintf(datestr,"%04d-%02d-%02d",now->tm_year+1900,now->tm_mon+1,now->tm_mday);
    sprintf(timestr,"%02d:%02d:%02d",now->tm_hour     ,now->tm_min  ,now->tm_sec );
    sprintf(mss,"%03d",tb.millitm);
    printf("%s %s.%s %s",datestr,timestr,mss,logstr);
    flog=fopen(logfilename1,"a");
    if (NULL!=flog) {
        fprintf(flog,"%s %s.%s %s",datestr,timestr,mss,logstr);
        if (ftell(flog)>MAXLOGSIZE) {
            fclose(flog);
            if (rename(logfilename1,logfilename2)) {
                remove(logfilename2);
                rename(logfilename1,logfilename2);
            }
            flog=fopen(logfilename1,"a");
            if (NULL==flog) return;
        }
        fclose(flog);
    }
}

void Log(const char *pszFmt,...) {
    va_list argp;

    LockIrq();
    va_start(argp,pszFmt);
    LogV(pszFmt,argp);
    va_end(argp);
    UnlockIrq();
}
