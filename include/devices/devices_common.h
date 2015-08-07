/*
 * devices_common.h
 *
 *  Created on: Oct 30, 2013
 *      Author: brent
 */

#ifndef DEVICES_COMMON_H_
#define DEVICES_COMMON_H_

#include "serial.h"

typedef struct _DeviceConfig {
    Serial *serial;
    char * buffer;
    size_t length;
} DeviceConfig;

typedef int (*DeviceSendSetupHandler)(DeviceConfig *, int);
typedef int (*DeviceSendCompleteHandler)(DeviceConfig *, int);

#define DEVICE_INIT_SUCCESS 			0
#define DEVICE_INIT_FAIL				1

#define DEVICE_STATUS_OK    			0
#define DEVICE_STATUS_DISCONNECTED		1
#define DEVICE_STATUS_FAULT             2


#endif /* DEVICES_COMMON_H_ */
