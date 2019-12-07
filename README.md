# Simple Serial Protocol for Arduino
Provides easy and robust general purpose serial communication between PC side applications and 
Arduino(-compatible) devices. This lib is the Arduino implementation of [Simple Serial Protocol].

## Install this Arduino library
### Manual method
Copy this repo folder into your arduino libraries location, and rename it to `SimpleSerialProtocol`. 
Your Arduino libraries folder depends on your operating system (and maybe custom path settings). Default path is:
#### Windows 7/8/10:
`C:\Users\<username>\Documents\Arduino\libraries\`
#### Linux
`/home/<username>/Arduino/libraries/`
#### macOS
`/Users/<username>/Documents/Arduino/libraries/`

## Usage example (echo_example sketch)
This example receives all supported datatype values from PC-side and sends them back immediately. 

```c++
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
    ssp.readCharArray(stringValue, stringBufferSize); // read chars from stream, fill buffer

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
    ssp.writeChar(charValue);
    ssp.writeCharArray(stringValue);

    ssp.writeEot(); // end command with end-of-transmission byte. important, don't forget!
}

void onError(unsigned int errorNum) {
    // error numbers are explained within Simple Serial Protocol's documentation at:
    // https://gitlab.com/yesbotics/simple-serial-protocol/simple-serial-protocol-docs
}
```

In this example, the text buffer is limited to 50 chars: `const int maxStringLength = 50;`.
Means **49** chars maximum text length. 
Amount of 49 characters should not be exceeded because 
1 byte is reserved for end-of-string byte.

This example can be found as Arduino sketch within the `simple-serial-protocol-arduino/examples/echo_example` folder.
It corresponds with Node.js echo example at [Simple Serial Protocol for Node.js].

## HardwareSerial and SoftwareSerial support
Just use [Serial (HardwareSerial)] (recommended) or [SoftwareSerial Library].
Both libs are based on [Arduino's Stream implementation].

## Arduino-CLI (arduino-cli) support 
We primarily compile and upload our Arduino sketches with [Arduino-CLI (arduino-cli)].
That project is great stuff. Fresh stuff.

## Limitations
Arduino device's memory is low.
Receiving long strings needs memory (1 byte per char) because of buffering every single character. 
Keep this in mind.
Flagships like Arduino Uno or Arduino Nano are powered by the [ATmega328P]), 
which are restricted to 2,048kB of internal memory. 

## Links
[Simple Serial Protocol]:https://gitlab.com/yesbotics/simple-serial-protocol/simple-serial-protocol-docs
[Simple Serial Protocol for Node.js]:https://gitlab.com/yesbotics/simple-serial-protocol/simple-serial-protocol-node
[Serial (HardwareSerial)]:https://www.arduino.cc/reference/en/language/functions/communication/serial/
[SoftwareSerial Library]:https://www.arduino.cc/en/Reference/SoftwareSerial
[Arduino's Stream implementation]:https://www.arduino.cc/reference/en/language/functions/communication/stream/
[Arduino-CLI (arduino-cli)]:https://github.com/arduino/arduino-cli
[ATmega328P]:https://www.microchip.com/wwwproducts/en/ATmega328p
