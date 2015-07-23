/**
 * AutoSport Labs - Race Capture Pro Firmware
 *
 * Copyright (C) 2014 AutoSport Labs
 *
 * This file is part of the Race Capture Pro firmware suite
 *
 * This is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details. You should have received a copy of the GNU
 * General Public License along with this code. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include "devices_common.h"
#include "stddef.h"

typedef enum {
    WIFI_STATUS_NOT_INIT = 0,
    WIFI_STATUS_PROVISIONED,
    WIFI_STATUS_ERROR
} wifi_status_t;

wifi_status_t wifi_get_status();
int wifi_init_connection(DeviceConfig *config);
int wifi_disconnect(DeviceConfig *config);
int wifi_check_connection_status(DeviceConfig *config);

#endif /* BLUETOOTH_H_ */
