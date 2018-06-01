#include <SimpleSerialProtocol.h>

//Baudrate for Serial Connection
const long BAUDRATE = 115200;
//Maximum time between single characters
const long CHARACTER_TIMEOUT = 500;

SimpleSerialProtocol ssp;

void setup()
{
	//enjoy:
	//if first param equals 0 the default error function "SimpleSerialProtocol::onError())" is called
	//sends escaped error message back:
	//    schema:     !<error number>,<escaped riginal message>;
	//    example:    !1,messageIncomplete,d1'zw; //message not received altogether (depends on CHARACTER_TIMEOUT)
	//    example:    !2,invalidCommand,e2'zwei'3.0#; //command is invalid, because registerable commands only exist in the range of ASCII 48-122 (yes, commands range is restricted :)
	//    example:    !3,unregisteredCommand,d3'zwei'3.0#; //just not registered
	//    example:    !4,invlidArgumentsCount,d4'zw; // just dont send more arguments than expected
	//feel free for implementing your own error handler.
	//maybe implement your customized default message handler
	ssp.initialize(0, &Serial, BAUDRATE, CHARACTER_TIMEOUT);

	//from serial monitor send 'a;' without quotes
	ssp.registerCommand('a', onCommandA, 0);

	//from serial monitor send f.e. 'b1;' without quotes
	ssp.registerCommand('b', onCommandB, 1);

	//from serial monitor send f.e 'c999,-11,-13.7899,SomeWord,Small text without komma and semicolon.,1,0;' without quotes
	ssp.registerCommand('c', onCommandC, 7);

	ssp.registerCommand('d', onCommandD, 4);

}

void loop()
{
	ssp.receive();
}

void onError(const uint8_t errorNum, const uint8_t command, const uint8_t numValues)
{
	ssp.sendCommand('!', "errrorororororororo");
}

void onCommandA(const uint8_t errorNum, const uint8_t command, const uint8_t numValues)
{
	ssp.sendCommand('a', "Yeah, you've sent no values.");
}

void onCommandB(const uint8_t errorNum, const uint8_t command, const uint8_t numValues)
{
	int value = ssp.getIntValue();
	ssp.sendCommand('b', "Wow, you've sent the integer value: " + String(value));
}

void onCommandC(const uint8_t errorNum, const uint8_t command, const uint8_t numValues)
{
	String answer = "Your full message was:\r\n";
	answer += String(ssp.getMessage()) + "\r\n";
	answer += "\r\n";
	answer += "You've sent Some incredible values:\r\n";
	int firstIntValue = ssp.getIntValue();
	answer += String(firstIntValue) + "\r\n";
	int secondIntValue = ssp.getIntValue();
	answer += String(secondIntValue) + "\r\n";
	int sum = firstIntValue + secondIntValue;
	answer += String(sum) + "\r\n";
	answer += String(ssp.getFloatValue()) + "\r\n";
	answer += String(ssp.getStringValue()) + "\r\n";
	answer += String(ssp.getStringValue()) + "\r\n";
	answer += String(ssp.getBoolValue()) + "\r\n";
	answer += String(ssp.getBoolValue()) + "\r\n";
	ssp.sendCommand('c', answer);
}

void onCommandD(const uint8_t errorNum, const uint8_t command, const uint8_t numValues)
{
	ssp.sendCommand('d', "received d command");
}

