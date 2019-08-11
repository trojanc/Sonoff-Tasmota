/*
  user_config_override.h - user configuration overrides user_config.h for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _USER_CONFIG_OVERRIDE_H_
#define _USER_CONFIG_OVERRIDE_H_

// force the compiler to show a warning to confirm that this file is inlcuded
#warning **** user_config_override.h: Using Settings from this File ****

/*****************************************************************************************************\
 * USAGE:
 *   To modify the stock configuration without changing the user_config.h file:
 *   (1) copy this file to "user_config_override.h" (It will be ignored by Git)
 *   (2) define your own settings below
 *   (3) for platformio:
 *         define USE_CONFIG_OVERRIDE as a build flags.
 *         ie1 : export PLATFORMIO_BUILD_FLAGS='-DUSE_CONFIG_OVERRIDE'
 *         ie2 : enable in file platformio.ini "build_flags = -Wl,-Tesp8266.flash.1m0.ld -DUSE_CONFIG_OVERRIDE"
 *       for Arduino IDE:
 *         enable define USE_CONFIG_OVERRIDE in user_config.h
 ******************************************************************************************************
 * ATTENTION:
 *   - Changes to SECTION1 PARAMETER defines will only override flash settings if you change define CFG_HOLDER.
 *   - Expect compiler warnings when no ifdef/undef/endif sequence is used.
 *   - You still need to update user_config.h for major define USE_MQTT_TLS.
 *   - All parameters can be persistent changed online using commands via MQTT, WebConsole or Serial.
\*****************************************************************************************************/

// -- Master parameter control --------------------
#undef  CFG_HOLDER
#define CFG_HOLDER        0x20190127             // [Reset 1] Change this value to load SECTION1 configuration parameters to flash

// -- Setup your own Wifi settings  ---------------
#undef STA_SSID1
#define STA_SSID1              "TrojanCWIFI"

#undef STA_PASS1
#define STA_PASS1              "TrojanC2102"

#undef STA_SSID2
#define STA_SSID2              "TrojanC"

#undef STA_PASS2
#define STA_PASS2              "TrojanC2102"

// -- Setup your own MQTT settings  ---------------
#undef  MQTT_HOST
#define MQTT_HOST         "ardumation.trojanc.co.za"

#undef  MQTT_PORT
#define MQTT_PORT         1883

#undef  MQTT_USER
#define MQTT_USER         "" 

#undef  MQTT_PASS
#define MQTT_PASS         ""         // [MqttPassword] Optional password

#undef  MQTT_FULLTOPIC
#define MQTT_FULLTOPIC         "TROJANC/%prefix%/%topic%/"

// -- HTTP ----------------------------------------
#undef  WEB_USERNAME
#define WEB_USERNAME         "trojanc"

#undef  WEB_PASSWORD
#define WEB_PASSWORD         "password"


// #undef  APP_TIMEZONE
// #define APP_TIMEZONE 2

#undef APP_LEDSTATE
#define APP_LEDSTATE LED_POWER_MQTT
/*********************************************************************************************\
 * END OF SECTION 1
 *
 * SECTION 2
 * - Enable a feature by removing both // in front of it
 * - Disable a feature by preceding it with //
\*********************************************************************************************/


#undef USE_ADC_VCC
#define USE_ADC_BATTERY
// #define USE_ADC_TEMP
#undef USE_DOMOTICZ
#undef USE_HOME_ASSISTANT
#undef USE_TIMERS
#undef USE_RULES
#undef USE_I2C
#undef USE_MHZ19
#undef USE_SENSEAIR
#undef USE_PMS5003
#undef USE_NOVA_SDS
#undef USE_PZEM004T
#undef USE_SERIAL_BRIDGE
#undef USE_IR_REMOTE
#undef USE_IR_RECEIVE
#undef USE_WS2812
#undef USE_ARILUX_RF
#undef USE_SR04
#undef USE_EMULATION
#undef USE_TX20_WIND_SENSOR
#undef USE_RC_SWITCH

#endif  // _USER_CONFIG_OVERRIDE_H_
