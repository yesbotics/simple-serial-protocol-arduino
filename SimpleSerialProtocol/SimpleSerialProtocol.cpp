#include "Arduino.h"
#include "SimpleSerialProtocol.h"
#include <stdarg.h>

/*************************************************
 * PUBLIC
 *************************************************/

SimpleSerialProtocol::SimpleSerialProtocol()
{
}

void SimpleSerialProtocol::initialize(ErrorFunctionPointer errorFctPtr, HardwareSerial *serialPtr, long baudrate, unsigned long timeout)
{
	_serialPointer = serialPtr;
	_timeout = timeout;

	_errorFctPtr = errorFctPtr;

	serialPtr->begin(baudrate);

	for (int a = 0; a < FUNCTION_BUFFER_SIZE; a++)
	{
		_functionPointers[a] = 0;
		_functionNumArguments[a] = 0;
	}

	flush();
}

void SimpleSerialProtocol::registerCommand(const uint8_t command, FunctionPointer fctPtr, const uint8_t numArguments)
{
	uint8_t idx = command - FUNCTION_BUFFER_OFFSET;
	_functionPointers[command - FUNCTION_BUFFER_OFFSET] = fctPtr;
	_functionNumArguments[command - FUNCTION_BUFFER_OFFSET] = numArguments;
}

void SimpleSerialProtocol::onError(const uint8_t errorNum, const uint8_t command, const uint8_t numValues)
{
	String origMsg = getMessage();
	String msgCopy = origMsg;
	msgCopy.replace(";", "#");
	msgCopy.replace(",", "'");
	String send = "!" + String(errorNum) + "," + msgCopy + ";";
	_serialPointer->print(send);
}

void SimpleSerialProtocol::receive()
{

	if (_serialPointer->available() > 0)
	{

		bool msgComplete = false;
		unsigned long startTime = millis();

		while (!msgComplete)
		{
			if (_serialPointer->available() > 0)
			{

				uint8_t byte = _serialPointer->read();

				_buffer[_count] = byte;
				_count++;

				if (byte == BYTE_END)
				{
					countArguments();
					handleCommand(true);
					msgComplete = true;
					flush();
				}
			}
			else
			{
				unsigned long currentTime = millis();
				unsigned long timeDiff = currentTime - startTime;

				if (timeDiff > _timeout)
				{
					handleCommand(false);
					msgComplete = true;
					flush();
				}
			}
		}
	}
}

void SimpleSerialProtocol::flush()
{
	for (uint8_t i = 0; i < BUFFER_SIZE; i++)
	{
		_buffer[i] = 0;
	}
	_count = 0;
	_numValues = 0;
	_position = 1;
}

void SimpleSerialProtocol::countArguments()
{
	for (uint8_t i = 0; i < _count; i++)
	{
		uint8_t byte = _buffer[i];

		if (byte == BYTE_DELIMITER)
		{
			_numValues++;
		}
	}
	if (_numValues == 0 && _count > 2) _numValues = 1;
	else if (_numValues > 0) _numValues++;
}

void SimpleSerialProtocol::handleCommand(const bool msgComplete)
{
	if (!msgComplete)
	{
		//_serialPointer->print("incomplete Message > Timeout");
		if (_errorFctPtr != 0) _errorFctPtr(1, '!', 0);
		else onError(1, '!', 0);
		return;
	}

	//get CommandChar from buffer
	uint8_t command = _buffer[0];
	uint8_t idx = command - FUNCTION_BUFFER_OFFSET;

	//is command valid (ASCII-range 48-122)
	bool isValidCommand = command >= FUNCTION_BUFFER_OFFSET && command <= 122;
	if (!isValidCommand)
	{
		//_serialPointer->print("invalidCommand");
		if (_errorFctPtr != 0) _errorFctPtr(2, '!', 0);
		else onError(2, '!', 0);
		return;
	}

	//is command registered
	void (*FunctionPointer)(const uint8_t errorNum, const uint8_t command, const uint8_t numValues) = _functionPointers[idx];
	bool isRegisteredCommand = FunctionPointer != 0;
	if (!isRegisteredCommand)
	{
		//_serialPointer->print("unregisteredCommand");
		if (_errorFctPtr != 0) _errorFctPtr(3, '!', 0);
		else onError(3, '!', 0);
		return;
	}

	//correct number of arguments ?
	uint8_t numArguments = _functionNumArguments[idx];
	bool isCorrectArgumentNum = _numValues == numArguments;
	if (!isCorrectArgumentNum)
	{
		//_serialPointer->print("invalidArgCount ");
		if (_errorFctPtr != 0) _errorFctPtr(4, '!', 0);
		else onError(4, '!', 0);
		return;
	}

	FunctionPointer(0, command, _numValues);

}

String SimpleSerialProtocol::getMessage()
{
	char txt[_count + 2];

	for (int pos = 0; pos < _count; pos++)
	{
		txt[pos] = _buffer[pos];
	}
	txt[_count] = '\0';

	String msg = String(txt);
	return msg;
}

String SimpleSerialProtocol::getStringValue()
{

	int start = _position;

	unsigned int len = 0;
	for (int pos = start; pos < _count; pos++)
	{
		uint8_t byte = _buffer[pos];

		char cbyte = _buffer[pos];

		if (byte == BYTE_DELIMITER)
		{
			len = pos - start;
			_position = ++pos;
			break;
		}
		else if (pos == _count - 1)
		{
			len = pos - start;
			_position = pos;
		}
	}

	char txt[len + 1];

	for (unsigned int i = 0; i < len; i++)
	{
		txt[i] = _buffer[start + i];
	}
	txt[len] = '\0';

	String value = String(txt);
	return value;
}

int SimpleSerialProtocol::getIntValue()
{
	return getStringValue().toInt();
}

float SimpleSerialProtocol::getFloatValue()
{
	String strValue = getStringValue();
	char charArray[strValue.length() + 1];
	strValue.toCharArray(charArray, sizeof(charArray));
	return atof(charArray);
}

bool SimpleSerialProtocol::getBoolValue()
{
	return getIntValue()==1;
}

void SimpleSerialProtocol::send(String msg)
{
	_serialPointer->print(msg);
}

void SimpleSerialProtocol::sendln(String msg)
{
	_serialPointer->println(msg);
}
