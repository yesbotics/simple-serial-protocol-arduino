/*
 *  Created on: 30.03.2019
 *      Author: Albrecht Nitsche und Jens Kabisch
 */

#ifndef CORE_H
#define CORE_H

#include <Arduino.h>
#include "common.h"

constexpr byte CHAR_NULL = 0x00; // 0 // End of String
constexpr uint8_t MAX_CHARARRAY_LENGTH = UINT8_MAX; // 255

typedef union
{
    float floatingPoint;
    uint8_t binary[4];
} BinaryFloat;

class Core
{
public:
    virtual void init();

    // single byte value
    byte readByte();
    void writeByte(byte bite) const;

    // multi bytes value
    void readBytes(byte* bites, size_t size);
    void writeBytes(const byte* bites,size_t size) const;

    // single bool value
    bool readBool();
    void writeBool(bool b) const;

    // single character
    char readChar();
    void writeChar(char ch) const;

    // text / string - char[]
    virtual bool readCString(char* output, uint8_t maxLength);
    void writeCString(const char* charArray) const;

    // text / string - String
    String readString(uint8_t maxLength = MAX_CHARARRAY_LENGTH);
    void writeString(const String& string) const;

    // -128 to 127
    int8_t readInt8();
    void writeInt8(int8_t num) const;

    // 0 to 255
    uint8_t readUnsignedInt8();
    void writeUnsignedInt8(uint8_t num) const;

    // -32,768 to 32,767
    int16_t readInt16();
    void writeInt16(int16_t num) const;

    // 0 to 65,523
    uint16_t readUnsignedInt16();
    void writeUnsignedInt16(uint16_t num) const;

    // -2,147,483,648 to 2,147,483,647
    int32_t readInt32();
    void writeInt32(int32_t num) const;

    // 0 to 4,294,967,295
    uint32_t readUnsignedInt32();
    void writeUnsignedInt32(uint32_t num) const;

    // -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
    int64_t readInt64();
    void writeInt64(int64_t num) const;

    // 0 to 18,446,744,073,709,551,615
    uint64_t readUnsignedInt64();
    void writeUnsignedInt64(uint64_t num) const;

    // -3.402,823,4 * 10^38 to 3.402,823,4 * 10^38
    float readFloat();
    void writeFloat(float f) const;

protected:
    Core(Stream& streamRef, bool isSoftwareSerial, unsigned long baudrate, unsigned long waitForByteTimeout);
    Core(Stream* streamPtr, bool isSoftwareSerial, unsigned long baudrate, unsigned long waitForByteTimeout);
    ~Core();
    Stream* streamPointer = nullptr;
    virtual void _onWaitForByteTimeout() = 0;

private:
    bool isSoftwareSerial;
    unsigned long baudrate;
    unsigned long waitForByteTimeout;

    void waitForBytes(int numBytes);
    void readSignedBytes(int8_t* buffer, size_t n) const;
};

#endif
