#include "AppError.h"

static char Canot_Translate[2][9] = {
		"unknow",
		"unsupport",
};

#define APP_ERROR_MAX (6)
/*
#define APP_SUCCESSED       0
#define APP_ERROR           1
#define APP_BEYOND_SCOPE    2
#define APP_POINTER_NULL    3
#define APP_ALREADY_DONE_BEFORE 	4
*/
static char ErrorTranslate[APP_ERROR_MAX][30]={
	"APP_SUCCESSED",
    "APP_ERROR",
	"APP_BEYOND_SCOPE",
    "APP_POINTER_NULL",
	"APP_ALREADY_DONE_BEFORE",
	"APP_PARAM_ERROR",
};
char *AppErrorGet(uint32_t error,char* p_cannot)
{
		if(error >= APP_ERROR_MAX) {return &Canot_Translate[0][0];}
		return &ErrorTranslate[error][0];
}
