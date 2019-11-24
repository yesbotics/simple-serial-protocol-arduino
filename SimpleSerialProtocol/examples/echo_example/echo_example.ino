#include <SimpleSerialProtocol.h>

// declare callbacks
void onError(unsigned int errorNum);

void onReceivedSomething();

// declare communication constants
const long BAUDRATE = 9600;
const long CHARACTER_TIMEOUT = 500;
const char COMMAND_ID_RECEIVE = 'r';
const char COMMAND_ID_SEND = 's';

// create instance. define command id range in which ssp is listening
SimpleSerialProtocol ssp(Serial, BAUDRATE, CHARACTER_TIMEOUT, onError, 'a', 'z');

void setup() {
    // init ssp. ssp calls Serial.begin(9600) behind the scenes
    ssp.init();
    // if message command with 'r' is received, callback will be called
    ssp.registerCommand(COMMAND_ID_RECEIVE, onReceivedSomething);
}

void loop() {
    // polling if received bytes available
    ssp.loop();
}

void onReceivedSomething() {
    //buffer and read sent string
    const int maxStringLength = 50;
    const char someString[maxStringLength];
    ssp.readCharArray(someString, maxStringLength);

    // read received float
    float someFloatingPointValue = ssp.readFloat();

    //send answer
    ssp.writeCommand(COMMAND_ID_SEND);
    ssp.writeFloat(someFloatingPointValue);
    ssp.writeCharArray(someString);
    ssp.writeEot(); //end of transmission
}

void onError(unsigned int errorNum) {
    //do nothing
}