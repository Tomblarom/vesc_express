/*
	Copyright 2026 Benjamin Vedder	benjamin@vedder.se

	This file is part of the VESC firmware.

	The VESC firmware is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The VESC firmware is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

/* ESP32C3 Supermini
   - https://www.fambach.net/esp32-c3-super-mini-board/
   - https://forum.esk8.news/t/usb-c-to-vesc-can-forward-adapter-cheap-and-diy/79789
   
   HW_HEADER=hwconf/devboards/hw_c3_supermini.h && HW_SRC=hwconf/devboards/hw_c3_supermini.c && idf.py build
   */

#ifndef MAIN_HWCONF_C3_SUPERMINI_H_
#define MAIN_HWCONF_C3_SUPERMINI_H_

#include "driver/gpio.h"

#define HW_NAME						"C3 Supermini"
#define HW_DEFAULT_ID				2

#define HW_INIT_HOOK()				hw_init()

// LEDs
// Blue is shared (GPIO8/SCK/D8), Red is not controllable (Power LED)
// On the VESC-Express, blue LED is used for feedback of BT and red for Wifi
#define LED_BLUE_PIN				8

#define LED_BLUE_ON()				gpio_set_level(LED_BLUE_PIN, 1)
#define LED_BLUE_OFF()				gpio_set_level(LED_BLUE_PIN, 0)

// CAN
#define CAN_TX_GPIO_NUM				1
#define CAN_RX_GPIO_NUM				0

// SD-card (external flash)
// #define SD_PIN_MOSI					4
// #define SD_PIN_MISO					6
// #define SD_PIN_SCK					5
// #define SD_PIN_CS					7

// SD-card (internal flash, use partition_storage.csv)
//  Warning: This option increases wear on the flash sectors and is only intended as a workaround to dedicate an entire dev-board for logging
//  data, without external SD-card. The ESP32C3 has only 4MB of internal flash and by default no space is left (see partition_ota.csv). When
//  enabling this option, the partitions for lisp, qml and ota_1 are replaced by one single partition. The downsides are that qml&lisp functionality
//  is disabled and the chip can be bricked, if an OTA update fails. However it's easier and cheaper to replace a $3 dev-board than the STM32F405 MCU.
#define HW_USE_FLASH_AS_SD 1

// UART
#define HW_NO_UART

// Functions
void hw_init(void);

#endif
