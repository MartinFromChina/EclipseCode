#ifndef __DATAFLOW_H
#define __DATAFLOW_H

#include "..\..\..\CommonSource\KeilMDK.h"

typedef enum
{
	CommandGetEntry = 0,  //
	CommandSetEntry,   //
	CharKeyEntry,    //  can be lose
	CharAirMouseEntry, //  can be lose
	CharPenTipEntry, //  can be lose
	CharColorEntry,
	CharPenTypeEntry,
	CharBatteryEntry,
}DataFlowEntry;

#define CHAR_KEY_LENGTH				3
#define CHAR_PENTIP_LENGTH			11
#define CHAR_AIRMOUSE_LENGTH		20
#define CHAR_COLOR_LENGTH			3
#define CHAR_PENTYPE_LENGTH			1
#define CHAR_BATTERY_LENGTH			1
#define MAX_DATA_LENGTH  			30

X_Boolean DataFlowPush(DataFlowEntry entry,const uint8_t * p_data,uint8_t length);
X_Boolean DataFlowPop(DataFlowEntry entry,uint8_t ** p_data,uint8_t* length);
X_Void DataFlowClear(DataFlowEntry entry);
X_Void DataFlowInit(X_Void);

#endif

