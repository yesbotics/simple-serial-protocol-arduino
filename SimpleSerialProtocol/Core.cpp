#include <Core.h>

Core::Core(Stream &streamRef, unsigned int streamType, unsigned long baudrate, unsigned long waitForByteTimeout) {
    this->streamPointer = &streamRef;
    this->afterConstructor(streamType, baudrate, waitForByteTimeout);
}

Core::Core(Stream *streamPtr, unsigned int streamType, unsigned long baudrate, unsigned long waitForByteTimeout) {
    this->streamPointer = streamPtr;
    this->afterConstructor(streamType, baudrate, waitForByteTimeout);
}

Core::~Core() {

}

void Core::afterConstructor(unsigned int streamType, unsigned long baudrate, unsigned long waitForByteTimeout) {
    this->streamType = streamType;
    this->baudrate = baudrate;
    this->waitForByteTimeout = waitForByteTimeout;
}

void Core::init() {

    if (this->streamType == STREAM_TYPE_HARDWARESERIAL) {
        ((HardwareSerial *) this->streamPointer)->begin(this->baudrate);
        return;
    }

#ifdef SOFTWARESERIAL_SUPPORTED
    if (this->streamType == STREAM_TYPE_SOFTWARESERIAL) {
        ((SoftwareSerial *) this->streamPointer)->begin(this->baudrate);
    }
#endif
}

byte Core::readByte() {
    return (byte) this->readInt8();
}

void Core::writeByte(const byte bite) {
    this->writeInt8(bite);
}

bool Core::readBool() {
    return (bool) (this->readInt8() != 0);
}

void Core::writeBool(const bool b) {
    this->writeInt8(b);
}

char Core::readChar() {
    return (char) this->readInt8();
}

void Core::writeChar(const char ch) {
    this->writeInt8(ch);
}

int8_t Core::readInt8() {
    this->waitForBytes(1); // Wait for 1 byte
    return (int8_t) this->streamPointer->read();
}

void Core::writeInt8(const int8_t num) {
    this->streamPointer->write(num);
}

uint8_t Core::readUnsignedInt8() {
    return (uint8_t) this->readInt8();
}

void Core::writeUnsignedInt8(const uint8_t num) {
    this->writeInt8(num);
}

int16_t Core::readInt16() {
    int8_t buffer[2];
    this->waitForBytes(2); // Wait for 2 bytes
    this->readSignedBytes(buffer, 2);
    return (((int16_t) buffer[0]) & 0xff) | (((int16_t) buffer[1]) << 8 & 0xff00);
}

void Core::writeInt16(const int16_t num) {
    int8_t buffer[2] = {(int8_t) (num & 0xff), (int8_t) (num >> 8)};
    this->streamPointer->write((uint8_t *) &buffer, 2 * sizeof(int8_t));
}

uint16_t Core::readUnsignedInt16() {
    return (uint16_t) this->readInt16();
}

void Core::writeUnsignedInt16(const uint16_t num) {
    this->writeInt16(num);
}

int32_t Core::readInt32() {
    int8_t buffer[4];
    this->waitForBytes(4); // Wait for 4 bytes
    this->readSignedBytes(buffer, 4);
    return (((int32_t) buffer[0]) & 0xff) | (((int32_t) buffer[1]) << 8 & 0xff00) |
           (((int32_t) buffer[2]) << 16 & 0xff0000) | (((int32_t) buffer[3]) << 24 & 0xff000000);
}

void Core::writeInt32(const int32_t num) {
    int8_t buffer[4] = {(int8_t) (num & 0xff), (int8_t) (num >> 8 & 0xff), (int8_t) (num >> 16 & 0xff),
                        (int8_t) (num >> 24 & 0xff)};
    this->streamPointer->write((uint8_t *) &buffer, 4 * sizeof(int8_t));
}

uint32_t Core::readUnsignedInt32() {
    return (uint32_t) this->readInt32();
}

void Core::writeUnsignedInt32(const uint32_t num) {
    this->writeInt32(num);
}

float Core::readFloat() {
    BinaryFloat binaryFloat;
    this->waitForBytes(4);
    this->readSignedBytes((int8_t *) binaryFloat.binary, 4);
    return binaryFloat.floatingPoint;
}

void Core::writeFloat(const float f) {
    BinaryFloat binaryFlaot;
    binaryFlaot.floatingPoint = f;
    this->streamPointer->write(binaryFlaot.binary, 4);
}

void Core::readCharArray(char *output, uint8_t maxLength) {
    bool stringComplete = false;
    output[0] = CHAR_NULL;
    uint8_t i = 0;
    while (!stringComplete && i < maxLength) {
        this->waitForBytes(1);
        char currentChar = (char) this->streamPointer->read();
        output[i] = currentChar;
        stringComplete = currentChar == CHAR_NULL;
        i++;
    }
}

void Core::writeCharArray(const char *charArray) {
    uint8_t i = 0;
    const uint8_t max = 250;
    char c = charArray[i];
    while (c != '\0' && i < max) {
        this->streamPointer->write(c);
        i++;
        c = charArray[i];
    }
    this->streamPointer->write(CHAR_NULL);
}

/***************************** PRIVATE *********************************/

void Core::waitForBytes(const int numBytes) {
    unsigned long startTime = millis();
    unsigned long timeout = this->waitForByteTimeout * numBytes;
    while (this->streamPointer->available() < numBytes) {
        if (millis() - startTime >= timeout) {
            this->onWaitForByteTimeout();
            return;
        }
    }
}

// NOTE : Serial.readBytes is SLOW
// this one is much faster, but has no timeout
void Core::readSignedBytes(int8_t *buffer, size_t n) {
    size_t i = 0;
    int c;
    while (i < n) {
        c = this->streamPointer->read();
        if (c < 0)
            break;
        *buffer++ = (int8_t) c; // buffer[i] = (int8_t)c;
        i++;
    }
}
