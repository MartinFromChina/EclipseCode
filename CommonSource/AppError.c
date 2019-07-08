#include "AppError.h"

static char Canot_Translate[2][9] = {
		"unknow",
		"unsupport",
};

#define APP_ERROR_MAX (4)
/*
#define APP_SUCCESSED       0
#define APP_BEYOND_SCOPE    1
#define APP_ERROR           2
#define APP_POINTER_NULL    3
*/
static char ErrorTranslate[APP_ERROR_MAX][30]={
	"APP_SUCCESSED",
    "APP_BEYOND_SCOPE",
    "APP_ERROR",
    "APP_POINTER_NULL",
};
char *AppErrorGet(uint32_t error,char* p_cannot)
{
		if(error >= APP_ERROR_MAX) {return &Canot_Translate[0][0];}
		return &ErrorTranslate[error][0];
}
