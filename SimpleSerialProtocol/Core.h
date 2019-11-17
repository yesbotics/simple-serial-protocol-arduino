/*
 *  Created on: 30.03.2019
 *      Author: Albrecht Nitsche und Jens Kabisch
 */

#ifndef CORE_H
#define CORE_H

#include <Arduino.h>

#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO)
#define SOFTWARESERIAL_SUPPORTED true
#endif

const unsigned int STREAM_TYPE_HARDWARESERIAL = 1;

#ifdef SOFTWARESERIAL_SUPPORTED
const unsigned int STREAM_TYPE_SOFTWARESERIAL = 2;
#endif

const byte CHAR_NULL = 0x00; // 0 // End of String

#ifdef SOFTWARESERIAL_SUPPORTED
#include <SoftwareSerial.h>
#endif

typedef union {
	float floatingPoint;
	uint8_t binary[4];
} BinaryFloat;

class Core {

public:
	void init();
	byte readByte();
	char readChar();bool readBool();
	int readInt();
	unsigned int readUnsignedInt();
	short readShort();
	unsigned short readUnsignedShort();
	long readLong();
	unsigned long readUnsignedLong();
	float readFloat();
	void readCharArray(char *output, uint8_t maxLength);
	void writeByte(const byte bite);
	void writeChar(const char ch);
	void writeBool(const bool b);
	void writeInt(const int i);
	void writeUnsignedInt(const unsigned int ui);
	void writeShort(const short sh);
	void writeUnsignedShort(const unsigned short ush);
	void writeLong(const long l);
	void writeUnsignedLong(const unsigned long ul);
	void writeFloat(const float f);
	void writeCharArray(const char *charArray);

protected:

	Core(Stream& streamRef, unsigned int streamType, unsigned long baudrate, unsigned long waitForByteTimeout);
	Core(Stream *streamPtr, unsigned int streamType, unsigned long baudrate, unsigned long waitForByteTimeout);
	~Core();
	Stream* streamPointer = 0;
	virtual void onWaitForByteTimeout() = 0;

private:

	unsigned int streamType = 0;
	unsigned long baudrate;
	unsigned long waitForByteTimeout;

	void afterConstructor(unsigned int streamType, unsigned long baudrate, unsigned long waitForByteTimeout);

	int8_t readInt8();
	int16_t readInt16();
	int32_t readInt32();
	void waitForBytes(int numBytes);
	void readSignedBytes(int8_t* buffer, size_t n);

	void writeInt8(const int8_t num);
	void writeInt16(const int16_t num);
	void writeInt32(const int32_t num);

};

#endif
