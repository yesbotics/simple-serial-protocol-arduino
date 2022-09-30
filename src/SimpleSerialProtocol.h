/*
 *  Created on: 02.02.2019
 *      Author: Albrecht Nitsche und Jens Kabisch
 */

#ifndef SIMPLE_SERIAL_PROTOCOL_H
#define SIMPLE_SERIAL_PROTOCOL_H

#include "Core.h"

const unsigned long FLUSH_TIMEOUT = 1000;
const unsigned long COMMAND_MESSAGE_COMPLETE_TIMEOUT = 1000;

const byte CHAR_EOT = 0x0A; // 10 // End of Transmission - Line Feed Zeichen \n

const byte COMMAND_CALLBACK_RANGE_FROM = 0;
const byte COMMAND_CALLBACK_RANGE_TO = 255;

const uint8_t ERROR_IS_DEAD = 255;
const uint8_t ERROR_EOT_WAS_NOT_READ = 254;
const uint8_t ERROR_IS_NOT_WAITING_FOR_READ_EOT = 253;
const uint8_t ERROR_IS_NOT_EOT = 252;

const uint8_t ERROR_WAIT_FOR_BYTE_TIMEOUT = 0;
const uint8_t ERROR_IS_NOT_INITIALIZED = 1;
const uint8_t ERROR_COMMAND_RANGE_IS_INVALID = 2;
const uint8_t ERROR_COMMAND_IS_NOT_IN_RESERVED_RANGE = 3;
const uint8_t ERROR_COMMAND_IS_REGISTERED = 4;
const uint8_t ERROR_COMMAND_IS_NOT_REGISTERED = 5;
const uint8_t ERROR_END_OF_STRING_BYTE_NOT_IN_CHAR_ARRAY = 6;


class SimpleSerialProtocol : public Core {

public:
    typedef void (*CallbackPointer)();

    typedef void (*ErrorCallbackPointer)(uint8_t errorNum);


#ifdef SOFTWARESERIAL_SUPPORTED

    SimpleSerialProtocol(
            SoftwareSerial& softwareSerialRef,
            unsigned long baudrate,
            unsigned long waitForByteTimeout,
            ErrorCallbackPointer errorCallbackPointer,
            byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
            byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO);

    SimpleSerialProtocol(
            SoftwareSerial* softwareSerialPtr,
            unsigned long baudrate,
            unsigned long waitForByteTimeout,
            ErrorCallbackPointer errorCallbackPointer,
            byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
            byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO);

#endif

    SimpleSerialProtocol(
            HardwareSerial& hardwareSerialRef,
            unsigned long baudrate,
            unsigned long waitForByteTimeout,
            ErrorCallbackPointer errorCallbackPointer,
            byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
            byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    );

    SimpleSerialProtocol(
            HardwareSerial* hardwareSerialPtr,
            unsigned long baudrate,
            unsigned long waitForByteTimeout,
            ErrorCallbackPointer errorCallbackPointer,
            byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
            byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    );

    SimpleSerialProtocol(
            Stream& streamRef,
            uint8_t streamType,
            unsigned long baudrate,
            unsigned long waitForByteTimeout,
            ErrorCallbackPointer errorCallbackPointer,
            byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
            byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    );

    SimpleSerialProtocol(
            Stream* streamPtr,
            uint8_t streamType,
            unsigned long baudrate,
            unsigned long waitForByteTimeout,
            ErrorCallbackPointer errorCallbackPointer,
            byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
            byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    );

    ~SimpleSerialProtocol();

    void init(int8_t rxPin = -1, int8_t txPin = -1);

    void setDieInstantlyOnNotRegisteredCommand(bool die);

    void registerCommand(byte command, CallbackPointer commandCallbackPointer);

    //overrideable
    virtual bool loop();

    byte readCommand();

    byte readEot();

    void writeCommand(byte command);

    void writeEot();

    // Override from Core
    bool readCString(char *output, uint8_t maxLength);

protected:

    byte commandCallbackRangeFrom;
    byte commandCallbackRangeTo;

    bool _isInitialized = false;
    bool _isDead = false;

    bool isCommandRangeValid(); //
    bool isCommandInReservedRange(byte command); //
    bool isCommandRegistered(byte command); //

    void onWaitForByteTimeout();

    virtual void onGotCommandByte(byte command);

    void registerCommandCallback(byte command, CallbackPointer commandCallbackPointer); //

    uint8_t getCommandIndex(byte command); //
    void callCommandCallback(byte command);

    void error(uint8_t errorNum, bool dieInstantly); //
    void flushCommand();

private:

    ErrorCallbackPointer errorCallbackPointer = nullptr;
    CallbackPointer* commandCallbackPointers = nullptr;
    bool dieInstantlyOnNotRegisteredCommand = true;

    bool isWaitingForReadEot = false;

    void afterConstructor(
            ErrorCallbackPointer errorCallbackPointer,
            byte commandCallbackRangeFrom,
            byte commandCallbackRangeTo
    );

    void die();
};

#endif
