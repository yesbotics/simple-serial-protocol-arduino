
#include <SimpleSerialProtocol.h>

//Baudrate for Serial Connection
const long BAUDRATE = 115200;
//Maximum time between single characters
const long CHARACTER_TIMEOUT = 500;

SimpleSerialProtocol ssp;

void setup()
{
  ssp.initialize(0, &Serial, BAUDRATE, CHARACTER_TIMEOUT);

  //from serial monitor send 'a;' without quotes
  ssp.registerCommand('a', onReceivedSomeCommandWithNoValues, 0);

  //from serial monitor send f.e. 'b1;' without quotes
  ssp.registerCommand('b', onReceivedSomeCommandWithOneValue, 1);

  //from serial monitor send f.e 'c999,-11,-13.7899,SomeWord,Small text without komma and semicolon.,1,0;' without quotes
  ssp.registerCommand('c', onReceivedSomeCommandWith4WithArbitraryValues, 7);


  ssp.sendln("Hello Bitch!");

}

void loop()
{
  ssp.receive();
}

void onReceivedSomeCommandWithNoValues(const uint8_t errorNum, const uint8_t command, const uint8_t numValues)
{
  ssp.sendln("Yeah, you've sent no values.");
}

void onReceivedSomeCommandWithOneValue(const uint8_t errorNum, const uint8_t command, const uint8_t numValues)
{
  int value = ssp.getIntValue();
  ssp.sendln("Wow, you've sent the integer value \"" + String(value) + "\".");
}

void onReceivedSomeCommandWith4WithArbitraryValues(const uint8_t errorNum, const uint8_t command, const uint8_t numValues)
{
  ssp.sendln("You've sent Some incredible values:");

  int firstIntValue = ssp.getIntValue();
  ssp.sendln(String(firstIntValue));

  int secondIntValue = ssp.getIntValue();
  ssp.sendln(String(secondIntValue));

  int sum = firstIntValue + secondIntValue;
  ssp.sendln("Sum of both:");
  ssp.sendln(String(sum));

  ssp.sendln(String(ssp.getFloatValue()));
  ssp.sendln(String(ssp.getStringValue()));
  ssp.sendln(String(ssp.getStringValue()));
  ssp.sendln(String(ssp.getBoolValue()));
  ssp.sendln(String(ssp.getBoolValue()));
  ssp.sendln("");
  ssp.sendln("Your full message was:");
  ssp.sendln(String(ssp.getMessage()));
}



