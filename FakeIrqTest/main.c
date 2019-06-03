#include "user_irq.h"
#include <stdio.h>
#include "app_timer.h"

APP_TIMER_DEF(timer1);
APP_TIMER_DEF(timer2);

static void timer1_callback(void)
{
	printf("timer1 come\r\n");
}
static void timer2_callback(void)
{
	printf("timer2 come\r\n");
}

int main(int argc,char * argv[]) {

	setbuf(stdout,NULL);
    UserIrqCreated();// log function init is done here
    StartTimer();

    AppTimerCreated(timer1,500,timer1_callback);
    AppTimerCreated(timer2,2000,timer2_callback);
    AppTimerStart(timer1);
	AppTimerStart(timer2);
    while (1)
    {

    }
    UserIrqDelete();
    return 0;
}
