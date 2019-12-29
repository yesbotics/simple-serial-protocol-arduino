#include <SimpleSerialProtocol.h>

// declare callbacks (this is boilerplate code but needed for proper compilation of the sketch)
void onError(unsigned int errorNum);
void onReceivedSomething();

// inintialize hardware constants
const long BAUDRATE = 9600; // speed of serial connection
const long CHARACTER_TIMEOUT = 500; // wait max 500 ms between single chars to be received

// initialize command constants
const byte COMMAND_ID_RECEIVE = 'r';
const byte COMMAND_ID_SEND = 's';

// Create instance. Pass Serial instance. Define command-id-range within Simple Serial Protocol is listening (here: a - z)
SimpleSerialProtocol ssp(Serial, BAUDRATE, CHARACTER_TIMEOUT, onError, 'a', 'z'); // ASCII: 'a' - 'z' (26 byes of RAM is reserved)

// Aternatively you can create an instance of SoftwareSerial
// https://www.arduino.cc/en/Reference/SoftwareSerial
// #include <SoftwareSerial.h>
// SoftwareSerial swSerial(2, 3); // RX, TX
// SimpleSerialProtocol ssp(swSerial, BAUDRATE, CHARACTER_TIMEOUT, onError, 'a', 'z');

void setup() {
    // prepare LED and set it off
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    // init ssp. ssp is calling 'Serial.begin(9600)' behind the scenes
    ssp.init();
    // if message command with 'r' is received, the given callback will be called
    ssp.registerCommand(COMMAND_ID_RECEIVE, onReceivedValues);
}

void loop() {
    // polling for available bytes
    ssp.loop();
}

// callbacks implementation
void onReceivedValues() {

    //
    // Receive Data
    //
    byte byteValue = ssp.readByte(); // Arduino's byte: https://www.arduino.cc/reference/en/language/variables/data-types/byte/
    bool booleanValue = ssp.readBool();
    int8_t tinySignedInt = ssp.readInt8();
    uint8_t tinyUnsignedInt = ssp.readUnsignedInt8();
    int16_t smallSignedInt = ssp.readInt16();
    uint16_t smallUnsignedInt = ssp.readUnsignedInt16();
    int32_t signedInt = ssp.readInt32();
    uint32_t unsignedInt = ssp.readUnsignedInt32();
    int64_t bigSignedInt = ssp.readInt64();
    uint64_t bigUnsignedInt = ssp.readUnsignedInt64();
    float floatValue = ssp.readFloat();
    char charValue = ssp.readChar();

    // string is special, because of its variable length (overall maximum size is 255, means 254 characters text length)
    // performance note: in this example 50 bytes of Arduino's RAM is used - each time you read a string

    // max. 49 chars length, 1 byte is reserved for end of string byte
    const uint8_t stringBufferSize = 50;

    // you can interpret strings as
    // Arduino's String Object https://www.arduino.cc/reference/en/language/variables/data-types/stringobject/
    String text1 = ssp.readString(stringBufferSize);

    // or as a plain char array / c-string
    char text2[stringBufferSize]; // create buffer char array
    ssp.readCString(text2, stringBufferSize); // read chars from stream, fill buffer

    // again an Arduino String Object
    // this time without custom maximum length
    // max. 254 chars length, 1 byte is reserved for end of string byte
    String text3 = ssp.readString();

    ssp.readEot(); // read and expect the end-of-transmission byte. important, don't forget!

    //
    // Immediately send back all received and interpreted values
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
    ssp.writeInt64(bigSignedInt);
    ssp.writeUnsignedInt64(bigUnsignedInt);
    ssp.writeFloat(floatValue);
    ssp.writeChar(charValue);
    ssp.writeString(text1);
    ssp.writeCString(text2);
    ssp.writeString(text3);

    ssp.writeEot(); // end command with end-of-transmission byte. important, don't forget!
}

void onError(unsigned int errorNum) {
    digitalWrite(LED_BUILTIN, HIGH);
}
