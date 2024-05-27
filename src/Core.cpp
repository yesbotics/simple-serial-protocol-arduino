#include <Core.h>

Core::Core(
    Stream& streamRef,
    const bool isSoftwareSerial,
    const unsigned long baudrate,
    const unsigned long waitForByteTimeout
) :
    isSoftwareSerial{isSoftwareSerial},
    baudrate{baudrate},
    waitForByteTimeout{waitForByteTimeout}
{
    this->streamPointer = &streamRef;
}

Core::Core(
    Stream* streamPtr,
    const bool isSoftwareSerial,
    const unsigned long baudrate,
    const unsigned long waitForByteTimeout
) :
    isSoftwareSerial{isSoftwareSerial},
    baudrate{baudrate},
    waitForByteTimeout{waitForByteTimeout}
{
    this->streamPointer = streamPtr;
}

// TODO: remove/delete stream popinter? possible?
Core::~Core() = default;

void Core::init()
{
#ifdef SOFTWARESERIAL_SUPPORTED
    if (isSoftwareSerial)
    {
        ((SoftwareSerial*)this->streamPointer)->begin(static_cast<long>(this->baudrate));
        return;
    }
#endif

#ifdef HARDWARESERIAL
#ifdef ESP32
            ((HardwareSerial*)this->streamPointer)->begin(this->baudrate, SERIAL_8N1, rxPin, txPin);
#else
    ((HardwareSerial*)this->streamPointer)->begin(this->baudrate);
#endif
#endif

#ifdef USBAPISERIAL
    ((Serial_*)this->streamPointer)->begin(this->baudrate);
#endif
}

byte Core::readByte()
{
    return static_cast<byte>(this->readInt8());
}

void Core::writeByte(const byte bite) const
{
    this->writeInt8(static_cast<int8_t>(bite));
}

void Core::readBytes(byte* bites, const size_t size)
{
    for (unsigned int i = 0; i < size; i++)
    {
        const byte bite = this->readByte();
        bites[i] = bite;
    }
}

void Core::writeBytes(const byte* bites, const size_t size) const
{
    for (unsigned int i = 0; i < size; i++)
    {
        const byte bite = bites[i];
        this->writeByte(bite);
    }
}

bool Core::readBool()
{
    return (bool)(this->readInt8() != 0);
}

void Core::writeBool(const bool b) const
{
    this->writeInt8((int8_t)b);
}

char Core::readChar()
{
    return static_cast<char>(this->readInt8());
}

void Core::writeChar(const char ch) const
{
    this->writeInt8(ch);
}

int8_t Core::readInt8()
{
    this->waitForBytes(1); // Wait for 1 byte
    return (int8_t)this->streamPointer->read();
}

void Core::writeInt8(const int8_t num) const
{
    this->streamPointer->write(num);
}

uint8_t Core::readUnsignedInt8()
{
    return static_cast<uint8_t>(this->readInt8());
}

void Core::writeUnsignedInt8(const uint8_t num) const
{
    this->writeInt8(static_cast<int8_t>(num));
}

int16_t Core::readInt16()
{
    constexpr uint8_t bufferSize = 2;
    int8_t buffer[bufferSize];
    this->waitForBytes(bufferSize); // Wait for 2 bytes
    this->readSignedBytes(buffer, bufferSize);
    return (
        (((int16_t)buffer[0]) & 0xff) |
        (((int16_t)buffer[1]) << 8 & (int16_t)0xff00)
    );
}

void Core::writeInt16(const int16_t num) const
{
    constexpr uint8_t bufferSize = 2;
    int8_t buffer[bufferSize] = {
        (int8_t)(num & 0xff),
        (int8_t)(num >> 8)
    };
    this->streamPointer->write(reinterpret_cast<uint8_t*>(&buffer), bufferSize * sizeof(int8_t));
}

uint16_t Core::readUnsignedInt16()
{
    return static_cast<uint16_t>(this->readInt16());
}

void Core::writeUnsignedInt16(const uint16_t num) const
{
    this->writeInt16(static_cast<int16_t>(num));
}

int32_t Core::readInt32()
{
    constexpr uint8_t bufferSize = 4;
    int8_t buffer[bufferSize];
    this->waitForBytes(bufferSize); // Wait for 4 bytes
    this->readSignedBytes(buffer, bufferSize);
    return (
        (((int32_t)buffer[0]) & 0xff) |
        (((int32_t)buffer[1]) << 8 & 0xff00) |
        (((int32_t)buffer[2]) << 16 & 0xff0000) |
        (((int32_t)buffer[3]) << 24 & static_cast<int32_t>(0xff000000))
    );
}

void Core::writeInt32(const int32_t num) const
{
    constexpr uint8_t bufferSize = 4;
    int8_t buffer[bufferSize] = {
        (int8_t)(num & 0xff),
        (int8_t)(num >> 8 & 0xff),
        (int8_t)(num >> 16 & 0xff),
        (int8_t)(num >> 24 & 0xff)
    };
    this->streamPointer->write(reinterpret_cast<uint8_t*>(&buffer), bufferSize * sizeof(int8_t));
}

uint32_t Core::readUnsignedInt32()
{
    return static_cast<uint32_t>(this->readInt32());
}

void Core::writeUnsignedInt32(const uint32_t num) const
{
    this->writeInt32(static_cast<int32_t>(num));
}

int64_t Core::readInt64()
{
    constexpr uint8_t bufferSize = 8;
    int8_t buffer[bufferSize];
    this->waitForBytes(bufferSize); // Wait for 8 bytes
    this->readSignedBytes(buffer, bufferSize);
    return (
        (((int64_t)buffer[0]) & 0xff) |
        (((int64_t)buffer[1]) << 8 & 0xff00) |
        (((int64_t)buffer[2]) << 16 & 0xff0000) |
        (((int64_t)buffer[3]) << 24 & 0xff000000) |
        (((int64_t)buffer[4]) << 32 & 0xff00000000) |
        (((int64_t)buffer[5]) << 40 & 0xff0000000000) |
        (((int64_t)buffer[6]) << 48 & 0xff000000000000) |
        (((int64_t)buffer[7]) << 56 & static_cast<int64_t>(0xff00000000000000))
    );
}

void Core::writeInt64(const int64_t num) const
{
    constexpr uint8_t bufferSize = 8;
    int8_t buffer[bufferSize] = {
        (int8_t)(num & 0xff),
        (int8_t)(num >> 8 & 0xff),
        (int8_t)(num >> 16 & 0xff),
        (int8_t)(num >> 24 & 0xff),
        (int8_t)(num >> 32 & 0xff),
        (int8_t)(num >> 40 & 0xff),
        (int8_t)(num >> 48 & 0xff),
        (int8_t)(num >> 56 & 0xff)
    };
    this->streamPointer->write(reinterpret_cast<uint8_t*>(&buffer), bufferSize * sizeof(int8_t));
}

uint64_t Core::readUnsignedInt64()
{
    return static_cast<uint64_t>(this->readInt64());
}

void Core::writeUnsignedInt64(const uint64_t num) const
{
    this->writeInt64(static_cast<int64_t>(num));
}

float Core::readFloat()
{
    BinaryFloat binaryFloat;
    this->waitForBytes(4);
    this->readSignedBytes(reinterpret_cast<int8_t*>(binaryFloat.binary), 4);
    return binaryFloat.floatingPoint;
}

void Core::writeFloat(const float f) const
{
    BinaryFloat binaryFlaot;
    binaryFlaot.floatingPoint = f;
    this->streamPointer->write(binaryFlaot.binary, 4);
}

bool Core::readCString(char* output, const uint8_t maxLength)
{
    // TODO: can run infinitly, since there is no break condition
    bool stringComplete = false;
    output[0] = CHAR_NULL;
    uint8_t i = 0;
    while (!stringComplete && i < maxLength)
    {
        if (!this->waitForBytes(1))return false;
        const char currentChar = static_cast<char>(this->streamPointer->read());
        output[i] = currentChar;
        stringComplete = currentChar == CHAR_NULL;
        i++;
    }
    return stringComplete;
}

String Core::readString(const uint8_t maxLength)
{
    const uint8_t stringBufferSize = maxLength;
    char charArrayValue[stringBufferSize];
    const bool successfull = this->readCString(charArrayValue, stringBufferSize);
    return successfull ? String{charArrayValue} : String{};
}

void Core::writeCString(const char* charArray) const
{
    uint8_t i = 0;
    constexpr uint8_t max = MAX_CHARARRAY_LENGTH;
    char c = charArray[i];
    while (c != '\0' && i < max)
    {
        this->streamPointer->write(c);
        i++;
        c = charArray[i];
    }
    this->streamPointer->write(CHAR_NULL);
}

void Core::writeString(const String& string) const
{
    this->writeCString(string.c_str());
}

/***************************** PRIVATE *********************************/

bool Core::waitForBytes(const int numBytes)
{
    const unsigned long startTime = millis();
    const unsigned long timeout = this->waitForByteTimeout * numBytes;
    while (this->streamPointer->available() < numBytes)
    {
        if (millis() - startTime >= timeout)
        {
            this->_onWaitForByteTimeout();
            return false;
        }
    }
    return true;
}

// OPTIMZATION NOTE : Serial.readBytes is SLOW
// this one is much faster, but has no timeout
void Core::readSignedBytes(int8_t* buffer, const size_t n) const
{
    size_t i = 0;
    while (i < n)
    {
        const int c = this->streamPointer->read();
        if (c < 0)
            break;
        *buffer++ = static_cast<int8_t>(c); // buffer[i] = (int8_t)c;
        i++;
    }
}
