#include <Arduino.h>
#include <SimpleSerialProtocol.h>

SimpleSerialProtocol ssp(Serial, 115200, 500, nullptr);

void setup() {
    ssp.init();
}

void loop() {
// write your code here
}