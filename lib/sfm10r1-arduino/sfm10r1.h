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
#ifndef __SFM10R1_H__
#define __SFM10R1_H__
#include <Arduino.h>
#include <stdint.h>

#define SFM10R1_SOFTWARE "0"
#define SFM10R1_CONTACT_DETAILS "1"
#define SFM10R1_SIL_REV_LOWER "2"
#define SFM10R1_SIL_REV_UPPER "3"
#define SFM10R1_FIRMWARE_MAJOR "4"
#define SFM10R1_FIRMWARE_MINOR "5"
#define SFM10R1_FIRMWARE_VARIANT "7"
#define SFM10R1_FIRMWARE_VCS_VERSION "8"
#define SFM10R1_SIGFOX_LIBRARY_VERSION "9"
#define SFM10R1_DEVICE_ID "10"
#define SFM10R1_PAC "11"

// 10 Minutes
#define SFM10R1_SEND_DELAY 600000

class sfm10r1
{
public:
	// Create a new instance of a sensor monitor
	sfm10r1(Stream& serial);

	/**
	 * Get some information from the device and write it into the buffer
	 * This buffer should be at least 24(?) character long
	 */
	void getInformation(const char* type, char* buffer);

	/**
	 * Sends a message as 24 hexidecimal characters
	 * Returns true if the message was sent, false if not sent, or not allowed
	 * to send right now
	 */
	bool sendMessage(const char* data, int length);

	/**
	 *
	 */
	bool sendMessage(byte data[12], int length);

	/**
	 * Do a test AT command and expect OK
	 */
	bool commsCheck(void);

	/**
	 * Returns true if we are allowed to send another message
	 */
	bool isReady(void);

	void printInfo(void);

	int packData(byte buffer[12], int offset, float value);
	int packData(byte buffer[12], int offset, int32_t value);
	int packData(byte buffer[12], int offset, int16_t value);
	int packData(byte buffer[12], int offset, int8_t value);
	int packData(byte buffer[12], int offset, uint8_t value);
private:

	Stream *serial_port;
	uint32_t lastSent = 0;

	void getResponse(char* buffer);



	/**
	 * Returns true if we read okay from a response
	 */
	bool readOK(void);

	void byteToHex(byte *data, int len, char *buffer);
};

#endif /* __SFM10R1_H__ */
