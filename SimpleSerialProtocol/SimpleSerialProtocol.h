#ifndef SimpleSerialProtocol_H_
#define SimpleSerialProtocol_H_

#include <inttypes.h>
#include <WString.h>
#include <HardwareSerial.h>

const uint8_t BYTE_BREAK = '|';
const uint8_t BYTE_DELIMITER = ',';
const uint8_t BYTE_END = ';';

class SimpleSerialProtocol
{

	private:

		HardwareSerial *_serialPointer;

		unsigned long _timeout;
		const static uint8_t BUFFER_SIZE = 255;

		// 48-122 (in ascii: 0 - z) a-z, A-Z, 0-9,
		const static uint8_t FUNCTION_BUFFER_SIZE = 75;
		const static uint8_t FUNCTION_BUFFER_OFFSET = 48;

		uint8_t _byteCommand;
		uint8_t _buffer[BUFFER_SIZE];
		uint8_t _count;
		uint8_t _numValues;
		uint8_t _position;

		typedef void (*FunctionPointer)(const uint8_t errorNum, const uint8_t command, const uint8_t numValues);
		FunctionPointer _functionPointers[FUNCTION_BUFFER_SIZE];
		uint8_t _functionNumArguments[FUNCTION_BUFFER_SIZE];

		typedef void (*ErrorFunctionPointer)(const uint8_t errorNum, const uint8_t command, const uint8_t numValues);
		ErrorFunctionPointer _errorFctPtr;

		void onError(const uint8_t errorNum, const uint8_t command, const uint8_t numValues);
		void handleCommand(const bool msgComplete);
		void countArguments();

	public:
		SimpleSerialProtocol();
		void initialize(ErrorFunctionPointer errorFctPtr, HardwareSerial *serialPtr, long baudrate, unsigned long timeout);
		void receive();
		void registerCommand(const uint8_t command, FunctionPointer fctPtr, const uint8_t numArguments);
		void flush();
		String getMessage();
		String getStringValue();
		int getIntValue();
		bool getBoolValue();
		float getFloatValue();
		void send(String msg);
		void sendCommand(char commanChar, String values);
		void sendln(String msg);

};

#endif

