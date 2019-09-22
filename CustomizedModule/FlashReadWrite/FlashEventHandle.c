#include "FlashEventHandle.h"
/*
 ******************************************************************************************
 handle flash read write event,descripe in detail:
 1, handle basic  read or write request in queue(the queue only hold the buf pointer)
 2, handle user define sector base on MCU sector system
 ******************************************************************************************
 */
