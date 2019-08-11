#ifdef USE_SIGFOX_DRIVER
#include <sfm10r1.h>
#include <TasmotaSerial.h>
#define PIN_SIGFOX_RX 4
#define PIN_SIGFOX_TX 5

// Interval at which messages may be sent
const uint32_t sigfox_interval = 1000; 

// When the next message will be sent
uint32_t sigfox_next = sigfox_interval; // 10 minutes

TasmotaSerial sigfoxSerial(PIN_SIGFOX_RX, PIN_SIGFOX_TX);
sfm10r1 Sigfox(sigfoxSerial);

void sigfoxInit(void){
	snprintf_P(log_data, sizeof(log_data), PSTR("Sigfox Init"));
    AddLog(LOG_LEVEL_ERROR);
	sigfoxSerial.begin(9600);
}

void sigfoxSendTelemetry(void){
	snprintf_P(log_data, sizeof(log_data), PSTR("Sigfox Send"));
    AddLog(LOG_LEVEL_ERROR);
	byte data[12];
	int offset = 0;
	offset = Sigfox.packData(data, offset, ds18x20_sensor[0].temperature); // 0  + 4 = 4
	offset = Sigfox.packData(data, offset, ds18x20_sensor[1].temperature); // 4  + 4 = 8
	offset = Sigfox.packData(data, offset, ds18x20_sensor[2].temperature); // 8  + 4 = 12
	bool ok = Sigfox.sendMessage(data, 12);
	// offset = Sigfox.packData(data, offset, altitude);         // 9  + 2 = 11
	// offset = Sigfox.packData(data, offset, angle);            // 11 + 1 = 12
}

void sigfoxOnMqttData(){

}

#define XSNS_99

bool Xsns99(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_PRE_INIT:
      sigfoxInit();
      break;
    case FUNC_EVERY_SECOND:
		if(millis() >= sigfox_next){
    	  	sigfoxSendTelemetry();
			sigfox_next = millis() + sigfox_interval;
		}
      break;
  }
  return result;
}
#endif