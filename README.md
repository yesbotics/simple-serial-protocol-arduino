# Simple Serial Protocol for Arduino
Easy and robust general purpose serial communication for PC side applications and Arduino(-compatible) devices.
Arduino implementation of our [Simple Serial Protocol]

## install the Arduino lib first
there are two ways:
### manual method
copy `SimpleSerialProtocol` folder from this repo to ur arduino libraries folder
### Arduino app built-in Library Manager
navigate through menu`Tools->Library Manager` and search for `SimpleSerialProtocol` and install.
!!!!!!!! muss noch in die arduino registry
## usage example
This example receives two values from PC-side or other arduinos-microntrollers and sends them back. 
The first value is an text of max 50 chars length (in this example. more is possible).
The second value is an floating point value. We have choosen 3.14159265359.
This example can be found as Arduino sketch within the `simple-serial-protocol-arduino/examples/echo_example` folder.
This example corresponds with Node.js echo example at [Simple Serial Protocol for Node.js].

```c++
#include <SimpleSerialProtocol.h>

// declare callbacks (this is boilerplate code but needed for proper compilation of the sketch)
void onError(unsigned int errorNum);
void onReceivedSomething();

// inintialize hardware constants
const long BAUDRATE = 9600; // speed of serial connection
const long CHARACTER_TIMEOUT = 500; // wait max 500 ms between single chars to be received

// inintialize command constants
const char COMMAND_ID_RECEIVE = 'r';
const char COMMAND_ID_SEND = 's';

// create instance. pass Serial instance define command id range within ssp is listening
SimpleSerialProtocol ssp(Serial, BAUDRATE, CHARACTER_TIMEOUT, onError, 'a', 'z');

// alternatively u can create an instance of SoftwareSerial
// (https://www.arduino.cc/en/Reference/SoftwareSerial)
// example:
// #include <SoftwareSerial.h>
// SoftwareSerial swSerial(2, 3); // RX, TX
// SimpleSerialProtocol ssp(swSerial, BAUDRATE, CHARACTER_TIMEOUT, onError, 'a', 'z');

void setup() {
    // init ssp. ssp is calling Serial.begin(9600); behind the scenes
    ssp.init();
    // if message command with 'r' is received, callback will be called
    ssp.registerCommand(COMMAND_ID_RECEIVE, onReceivedSomething);
}

void loop() {
    // polling if received bytes available
    ssp.loop();
}

// declare callbacks
void onReceivedSomething() {

    // read and create buffer for received string
    const int maxStringLength = 50;
    char someString[maxStringLength];
    ssp.readCharArray(someString, maxStringLength);

    // read received float
    float someFloatingPointValue = ssp.readFloat();

    // read and expect the end-of-transmission byte. important, dont forget
    ssp.readEot();

    //send answer
    ssp.writeCommand(COMMAND_ID_SEND);
    ssp.writeCharArray(someString);
    ssp.writeFloat(someFloatingPointValue);
    ssp.writeEot(); //end of transmission
}

void onError(unsigned int errorNum) {
    // error numbers are explained within Simple Serial Protocol's documentation at:
    // https://gitlab.com/yesbotics/simple-serial-protocol/simple-serial-protocol-docs
}
```

## HardwareSerial and SoftwareSerial support
Just use [Serial (HardwareSerial)] (recommended) or [SoftwareSerial Library].
Both libs are based on [Arduino's Stream implementation].

## Arduino-CLI (arduino-cli) support 
We primarily compile and upload our Arduino sketches with [Arduino-CLI (arduino-cli)].
That project is great stuff. fresh stuff.

## Links
[Simple Serial Protocol]:https://gitlab.com/yesbotics/simple-serial-protocol/simple-serial-protocol-docs
[Simple Serial Protocol for Node.js]:https://gitlab.com/yesbotics/simple-serial-protocol/simple-serial-protocol-node
[Serial (HardwareSerial)]:https://www.arduino.cc/reference/en/language/functions/communication/serial/
[SoftwareSerial Library]:https://www.arduino.cc/en/Reference/SoftwareSerial
[Arduino's Stream implementation]:https://www.arduino.cc/reference/en/language/functions/communication/stream/
[Arduino-CLI (arduino-cli)]:https://github.com/arduino/arduino-cli
