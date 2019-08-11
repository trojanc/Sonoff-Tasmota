/**
MIT License

Copyright (c) 2017 TrojanC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "sfm10r1.h"


sfm10r1::sfm10r1(Stream& serial){
	serial_port = &serial;
}

void sfm10r1::getResponse(char* buffer){
	while (!serial_port->available()){
		delay(50); // Wait a bit for a response
	}

	byte length = 0;
	while(serial_port->available()){
		buffer[length++] = serial_port->read();
		delay(10);
	}
	// Replace the new line with a null ending
	buffer[length-2] = '\0';
}

void sfm10r1::getInformation(const char* type, char* buffer){
	serial_port->print(F("AT$I="));
	serial_port->print(type);
	serial_port->print(F("\r"));
	sfm10r1::getResponse(buffer);
}

bool sfm10r1::readOK(){
	char buffer[10];
	sfm10r1::getResponse(buffer);
	return strcmp("OK", buffer) == 0;
}

bool sfm10r1::commsCheck(){
	serial_port->print(F("AT\r"));
	return sfm10r1::readOK();
}

void sfm10r1::byteToHex(byte *data, int len, char *buffer){
    for (int i = 0; i < len; i++)
    {
        byte nib1 = (data[i] >> 4) & 0x0F;
        byte nib2 = (data[i] >> 0) & 0x0F;
        buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
        buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
    }
    buffer[len*2] = '\0';
}

bool sfm10r1::sendMessage(byte data[12], int length){
	bool maySend = sfm10r1::isReady();
	if(!maySend){
		Serial.println(F("Wait!"));
		return false;
	}
	char hexData[25];
	sfm10r1::byteToHex(data, length, hexData);
	return sfm10r1::sendMessage(hexData, length * 2);
}

bool sfm10r1::sendMessage(const char* data, int length){
	bool maySend = sfm10r1::isReady();
	if(!maySend){
		Serial.println(F("Wait!"));
		return false;
	}

	Serial.print(F("AT$SF="));
	Serial.write(data, length);
	Serial.println();

	serial_port->print(F("AT$SF="));
	serial_port->write(data, length);
	serial_port->print(F("\r"));

	delay(7000); // Wait at least 7 second for sending to complete

	bool ok = true; // sfm10r1::readOK();
	lastSent = millis();
	return ok;
}

bool sfm10r1::isReady(void){
	unsigned long currentTime = millis();
	if (lastSent == 0){
		return true;  //  First time sending.
	}
	const unsigned long elapsedTime = currentTime - lastSent;

	if (elapsedTime >= SFM10R1_SEND_DELAY){
		return true;
	}

	//  For development, allow sending every 10 seconds.
//	if (elapsedTime > 10 * 1000) {
//		return true;
//	}

	return false;
}

int sfm10r1::packData(byte buffer[12], int offset, float value){
	// Won't fit
	if(offset + 4 > 12){
		return -1;
	}
	typedef union
	{
	 float number;
	 uint8_t bytes[4];
	} FLOATUNION_t;
	FLOATUNION_t val;
	val.number = value;
	buffer[offset    ] = val.bytes[0];
	buffer[offset + 1] = val.bytes[1];
	buffer[offset + 2] = val.bytes[2];
	buffer[offset + 3] = val.bytes[3];
	return offset + 4;
}

int sfm10r1::packData(byte buffer[12], int offset, int32_t value){
	// Won't fit
	if(offset + 4 > 12){
		return -1;
	}

	buffer[offset    ] = (int)((value & 0xFF000000) >> 24 );
	buffer[offset + 1] = (int)((value & 0x00FF0000) >> 16 );
	buffer[offset + 2] = (int)((value & 0x0000FF00) >> 8 );
	buffer[offset + 3] = (int)((value & 0X000000FF));
	return offset + 4;
}

int sfm10r1::packData(byte buffer[12], int offset, int16_t value){
	// Won't fit
	if(offset + 2 > 12){
		return -1;
	}
	buffer[offset    ] = (int)((value & 0x0000FF00) >> 8 );
	buffer[offset + 1] = (int)((value & 0X000000FF));
	return offset + 2;
}

int sfm10r1::packData(byte buffer[12], int offset, int8_t value){
	// Won't fit
	if(offset + 1 > 12){
		return -1;
	}
	buffer[offset] = (int)((value & 0X000000FF));
	return offset + 1;
}

int sfm10r1::packData(byte buffer[12], int offset, uint8_t value){
	// Won't fit
	if(offset + 1 > 12){
		return -1;
	}
	buffer[offset] = (int)((value & 0X000000FF));
	return offset + 1;
}

void sfm10r1::printInfo(void){
	Serial.println("Getting info");
	char buffer[24];

	sfm10r1::getInformation(SFM10R1_SOFTWARE, buffer);
	Serial.print(F("Software: "));
	Serial.println(buffer);

	sfm10r1::getInformation(SFM10R1_CONTACT_DETAILS, buffer);
	Serial.print(F("Contact: "));
	Serial.println(buffer);

	Serial.print(F("SIL REV: "));
	sfm10r1::getInformation(SFM10R1_SIL_REV_UPPER, buffer);
	Serial.print(buffer);
	sfm10r1::getInformation(SFM10R1_SIL_REV_LOWER, buffer);
	Serial.println(buffer);

	Serial.print(F("Firmware: "));
	sfm10r1::getInformation(SFM10R1_FIRMWARE_MAJOR, buffer);
	Serial.print(buffer);
	sfm10r1::getInformation(SFM10R1_FIRMWARE_MINOR, buffer);
	Serial.println(buffer);

	sfm10r1::getInformation(SFM10R1_FIRMWARE_VARIANT, buffer);
	Serial.print(F("Variant: "));
	Serial.println(buffer);

	sfm10r1::getInformation(SFM10R1_FIRMWARE_VCS_VERSION, buffer);
	Serial.print(F("VCS: "));
	Serial.println(buffer);

	sfm10r1::getInformation(SFM10R1_SIGFOX_LIBRARY_VERSION, buffer);
	Serial.print(F("Library: "));
	Serial.println(buffer);

	sfm10r1::getInformation(SFM10R1_DEVICE_ID, buffer);
	Serial.print(F("DeviceId: "));
	Serial.println(buffer);

	sfm10r1::getInformation(SFM10R1_PAC, buffer);
	Serial.print(F("PAC: "));
	Serial.println(buffer);
}
