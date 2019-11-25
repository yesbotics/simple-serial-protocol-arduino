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

// create instance. define command id range in which ssp is listening
SimpleSerialProtocol ssp(Serial, BAUDRATE, CHARACTER_TIMEOUT, onError, 'a', 'z');

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