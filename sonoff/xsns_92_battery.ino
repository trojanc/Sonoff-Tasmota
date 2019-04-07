/*
  xsns_91_thermistor.ino - Analog Thermistor sensor support for Sonoff-Tasmota

  Copyright (C) 2018  Nuno Ferreira and Theo Arends

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

#ifdef USE_ADC_BATTERY
#ifdef USE_ADC_VCC
#error "You cannot use both USE_ADC_BATTERY and USE_ADC_VCC"
#endif

void Sr92Show(boolean json)
{
	int rawLevel = analogRead(A0);

	// the 10kΩ/47kΩ voltage divider reduces the voltage, so the ADC Pin can handle it
	// According to Wolfram Alpha, this results in the following values:
	// 10kΩ/(47kΩ+10kΩ)*  5v = 0.8772v
	// 10kΩ/(47kΩ+10kΩ)*3.7v = 0.649v
	// 10kΩ/(47kΩ+10kΩ)*3.1v = 0.544
	// * i asumed 3.1v as minimum voltage => see LiPO discharge diagrams
	// the actual minimum i've seen was 467, which would be 2.7V immediately before automatic cutoff
	// a measurement on the LiPo Pins directly resulted in >3.0V, so thats good to know, but no danger to the battery.

	// convert battery level to percent
	int level = map(rawLevel, 500, 649, 0, 100);

	// i'd like to report back the real voltage, so apply some math to get it back
	// 1. convert the ADC level to a float
	// 2. divide by (R2[1] / R1 + R2)
	// [1] the dot is a trick to handle it as float
	float realVoltage = (float)rawLevel / 1000 / (10000. / (47000 + 10000));
	
    if(json) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"Battery\":{\"level\":%d,\"voltage\":%s,\"charging\":%d}"), mqtt_data, 
    		level < 150 ? level : 100,
			String(realVoltage, 3).c_str(),
			rawLevel > 800 ? 1 : 0);
#ifdef USE_WEBSERVER
    } else {
    	// const char HTTP_SNS_TEMP[] PROGMEM = "%s{s}%s " D_TEMPERATURE "{m}%s&deg;%c{e}";                             // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
    	snprintf_P(mqtt_data, sizeof(mqtt_data), "%s{s} Percent {m}%d{e}", mqtt_data, level < 150 ? level : 100);
    	snprintf_P(mqtt_data, sizeof(mqtt_data), "%s{s} Real {m}%s{e}", mqtt_data, String(realVoltage, 3).c_str());
    	snprintf_P(mqtt_data, sizeof(mqtt_data), "%s{s} Charging {m}%s{e}", mqtt_data, rawLevel > 800 ? "Yes" : "No");
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_92

bool Xsns92(byte function)
{
	bool result = false;

    switch (function) {
      case FUNC_JSON_APPEND:
        Sr92Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        Sr92Show(0);
        break;
#endif  // USE_WEBSERVER
  }
  return result;
}

#endif  // USE_ADC_TEMP
