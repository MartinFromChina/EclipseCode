#ifndef __USBFEATURETB_H
#define __USBFEATURETB_H

#include "..\..\..\..\CommonSource\KeilMDK.h"

typedef enum
{
	SetTestMode 			= 0,
	SetReportBleState 		= 0x81,
	SetReportProductType 	= 0x82,
	SetReportInformation 	= 0x83,
	SetKeyValueMap  		= 0x84,
	SetClearWhiteList 		= 0x85,
	SetUserID				= 0x86,
	SetPenType 				= 0x87,
	SetColor 				= 0x88,
	SetEnableHidDevice 		= 0x89,
	GetResult 				= 0xff,
}enum_UsbCommand;

X_Void GetFeatureAndSetFeature(X_Void);

#endif
