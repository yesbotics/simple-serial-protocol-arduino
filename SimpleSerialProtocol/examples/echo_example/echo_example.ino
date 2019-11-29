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

    // read and create buffer for received string
    const int maxStringLength = 50;
    char someString[maxStringLength];

    // reads the string from serial and writes it to 'someString'
    ssp.readCharArray(someString, maxStringLength);

    // read received float
    float someFloatingPointValue = ssp.readFloat();

    // read and expect the end-of-transmission byte. important, don't forget!
    ssp.readEot();

    //
    // Send answer
    //
    ssp.writeCommand(COMMAND_ID_SEND);
    ssp.writeCharArray(someString);
    ssp.writeFloat(someFloatingPointValue);
    ssp.writeEot(); //end of transmission
}

void onError(unsigned int errorNum) {
    // error numbers are explained within Simple Serial Protocol's documentation at:
    // https://gitlab.com/yesbotics/simple-serial-protocol/simple-serial-protocol-docs
}
