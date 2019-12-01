#include "Arduino.h"
#include <SimpleSerialProtocol.h>

const char CHAR = 's';
const unsigned long BAUDRATE = 9600;
const unsigned long CHARACTER_TIMEOUT = 500;

//HardwareSerial *serialPtr = &Serial;

void onFatalError(unsigned int errorNum);
void onHello();

SimpleSerialProtocol ssp(
        &Serial,
        BAUDRATE,
        CHARACTER_TIMEOUT,
        onFatalError,
        CHAR,
        CHAR
);

void setup() {
    ssp.init();
    ssp.registerCommand(CHAR,onHello);
}

void loop() {
    ssp.loop();
}

void onFatalError(unsigned int errorNum) {
    //dev_printFatalError(errorNum, Serial);
}

void onHello() {
    ssp.readEot();
    ssp.writeCommand(CHAR);
    ssp.writeEot();
}