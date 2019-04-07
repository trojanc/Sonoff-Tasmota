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

#ifdef USE_ADC_TEMP
#ifdef USE_ADC_VCC
#error "You cannot use both USE_ADC_TEMP and USE_ADC_VCC"
#endif

float Sr91CalculateTemp()
{
	uint16_t adc = AdcRead();
	float V_NTC = ((float)adc) / ADC_TEMP_RESOLUTION;
	float R_NTC = (ADC_TEMP_R2 * V_NTC) / (ADC_TEMP_VCC - V_NTC);
	R_NTC = log(R_NTC);
	float TEMP_K = 1.0 / (ADC_TEMP_SH_A + (ADC_TEMP_SH_B * R_NTC) + (ADC_TEMP_SH_C * R_NTC * R_NTC * R_NTC));
	return ConvertTemp(TEMP_K - 273.15);
}

void Sr91Show(boolean json)
{
	float t = Sr91CalculateTemp();
	char temperature[10];
	dtostrfd(t, TEMP_RESOLUTION, temperature);
	
    if(json) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"ADC_TEMP\":{\"" D_JSON_TEMPERATURE "\":%s}"), mqtt_data, temperature);
#ifdef USE_WEBSERVER
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TEMP, mqtt_data, "ADC", temperature, TempUnit());
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_91

bool Xsns91(byte function)
{
	bool result = false;

    switch (function) {
      case FUNC_JSON_APPEND:
        Sr91Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        Sr91Show(0);
        break;
#endif  // USE_WEBSERVER
  }
  return result;
}

#endif  // USE_ADC_TEMP
