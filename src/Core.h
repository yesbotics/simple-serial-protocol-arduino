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
const uint8_t MAX_CHARARRAY_LENGTH = UINT8_MAX; // 255

#ifdef SOFTWARESERIAL_SUPPORTED
#include <SoftwareSerial.h>
#endif

typedef union {
	float floatingPoint;
	uint8_t binary[4];
} BinaryFloat;

class Core {

public:
	virtual void init();

    // single byte value
	byte readByte();
	void writeByte(const byte bite);

    // single bool value
	bool readBool();
	void writeBool(const bool b);

    // single character
	char readChar();
	void writeChar(const char ch);

	// text / string - char[]
	virtual bool readCString(char *output, uint8_t maxLength);
	void writeCString(const char *charArray);

    // text / string - String
    String readString();
	void writeString(const String &string);

	// -128 to 127
    int8_t readInt8();
    void writeInt8(const int8_t num);

	// 0 to 255
    uint8_t readUnsignedInt8();
    void writeUnsignedInt8(const uint8_t num);

    // -32,768 to 32,767
    int16_t readInt16();
    void writeInt16(const int16_t num);

    // 0 to 65,523
    uint16_t readUnsignedInt16();
    void writeUnsignedInt16(const uint16_t num);

    // -2,147,483,648 to 2,147,483,647
    int32_t readInt32();
    void writeInt32(const int32_t num);

    // 0 to 4,294,967,295
    uint32_t readUnsignedInt32();
    void writeUnsignedInt32(const uint32_t num);

    // -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
    int64_t readInt64();
    void writeInt64(const int64_t num);

    // 0 to 18,446,744,073,709,551,615
    uint64_t readUnsignedInt64();
    void writeUnsignedInt64(const uint64_t num);

    // -3.402,823,4 * 10^38 to 3.402,823,4 * 10^38
	float readFloat();
	void writeFloat(const float f);

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

	void waitForBytes(int numBytes);
	void readSignedBytes(int8_t* buffer, size_t n);

};

#endif
