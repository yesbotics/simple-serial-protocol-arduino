#include <SimpleSerialProtocol.h>

// declare callbacks (this is boilerplate code but needed for proper compilation of the sketch)
void onError(unsigned int errorNum);
void onReceivedSomething();

// inintialize hardware constants
const long BAUDRATE = 9600; // speed of serial connection
const long CHARACTER_TIMEOUT = 500; // wait max 500 ms between single chars to be received

// initialize command constants
const char COMMAND_ID_RECEIVE = 'r';
const char COMMAND_ID_SEND = 's';

// Create instance. Pass Serial instance. Define command id range within Simple Serial Protocol is listening (here: a - z)
SimpleSerialProtocol ssp(Serial, BAUDRATE, CHARACTER_TIMEOUT, onError, 'a', 'z');

// alternatively u can create an instance of SoftwareSerial
// https://www.arduino.cc/en/Reference/SoftwareSerial
// #include <SoftwareSerial.h>
// SoftwareSerial swSerial(2, 3); // RX, TX
// SimpleSerialProtocol ssp(swSerial, BAUDRATE, CHARACTER_TIMEOUT, onError, 'a', 'z');

void setup() {
    // init ssp. ssp is calling 'Serial.begin(9600)' behind the scenes
    ssp.init();
    // if message command with 'r' is received, the given callback will be called
    ssp.registerCommand(COMMAND_ID_RECEIVE, onReceivedSomething);
}

void loop() {
    // polling for available bytes
    ssp.loop();
}

// callbacks implementation
void onReceivedSomething() {

    //
    // Receive data
    //

    byte byteValue = ssp.readByte();

    bool booleanValue = ssp.readBool();

    int8_t tinySignedInt = ssp.readInt8();

    uint8_t tinyUnsignedInt = ssp.readUnsignedInt8();

    int16_t smallSignedInt = ssp.readInt16();

    uint16_t smallUnsignedInt = ssp.readUnsignedInt16();

    int32_t signedInt = ssp.readInt32();

    uint32_t unsignedInt = ssp.readUnsignedInt32();

    float floatValue = ssp.readFloat();

    char charValue = ssp.readChar();

    const int stringBufferSize = 50; // means max. 49 chars length, 1 byte is reserved for end of string byte
    char stringValue[stringBufferSize]; // create buffer char array
    ssp.readCharArray(stringValue, stringBufferSize); // read vhars from stream, fill buffer

    ssp.readEot(); // read and expect the end-of-transmission byte. important, don't forget!


    //
    // Send answer
    //

    ssp.writeCommand(COMMAND_ID_SEND); // start command with command id

    ssp.writeByte(byteValue);

    ssp.writeBool(booleanValue);

    ssp.writeInt8(tinySignedInt);

    ssp.writeUnsignedInt8(tinyUnsignedInt);

    ssp.writeInt16(smallSignedInt);

    ssp.writeUnsignedInt16(smallUnsignedInt);

    ssp.writeInt32(signedInt);

    ssp.writeUnsignedInt32(unsignedInt);

    ssp.writeFloat(floatValue);

    ssp.writeCharArray(stringValue);

    ssp.writeEot(); // end command with end-of-transmission byte. important, don't forget!
}

void onError(unsigned int errorNum) {
    // error numbers are explained within Simple Serial Protocol's documentation at:
    // https://gitlab.com/yesbotics/simple-serial-protocol/simple-serial-protocol-docs
}
