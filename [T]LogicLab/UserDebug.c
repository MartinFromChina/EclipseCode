#include "UserDebug.h"

static char Canot_Translate[2][9] = {
		"unknow",
		"unsupport",
};

#define EVENT_ENTRY_MAX (8)
/*
	CommandGetEntry ,  //
	CommandSetEntry,   //
	CharKeyEntry,    //  can be lose
	CharAirMouseEntry, //  can be lose
	CharPenTipEntry, //  can be lose
	CharColorEntry,
	CharPenTypeEntry,
	CharBatteryEntry,
*/
static char EventTranslate[EVENT_ENTRY_MAX][30]={
	"CommandGetEntry",
    "CommandSetEntry",
	"CharKeyEntry",
    "CharAirMouseEntry",
	"CharPenTipEntry",
	"CharColorEntry",
	"CharPenTypeEntry",
	"CharBatteryEntry",
};
char *EventStringGet(uint8_t event)
{
		if(event >= EVENT_ENTRY_MAX) {return &Canot_Translate[0][0];}
		return &EventTranslate[event][0];
}

uint8_t GetDebugFlag(uint8_t number)
{
	if(number == 0) {return DATA_FLOW_GET_DEBUG;}
	else if(number == 1){return DATA_FLOW_SET_DEBUG;}
	else if(number == 2){return DATA_FLOW_KEY_DEBUG;}
	else if(number == 3){return DATA_FLOW_AIRMOUSE_DEBUG;}
	else if(number == 4){return DATA_FLOW_PENTIP_DEBUG;}
	else if(number == 5){return DATA_FLOW_COLOR_DEBUG;}
	else if(number == 6){return DATA_FLOW_PENTYPE_DEBUG;}
	else if(number == 7){return DATA_FLOW_BATTERY_DEBUG;}
	else {return 0;}
}
