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

const unsigned int COMMAND_CALLBACK_RANGE_FROM = 0;
const unsigned int COMMAND_CALLBACK_RANGE_TO = 255;

const unsigned int ERROR_IS_DEAD = 255;
const unsigned int ERROR_EOT_WAS_NOT_READ = 254;
const unsigned int ERROR_IS_NOT_WAITING_FOR_READ_EOT = 253;
const unsigned int ERROR_IS_NOT_EOT = 252;

const unsigned int ERROR_WAIT_FOR_BYTE_TIMEOUT = 0;
const unsigned int ERROR_IS_NOT_INITIALIZED = 1;
const unsigned int ERROR_COMMAND_RANGE_IS_INVALID = 2;
const unsigned int ERROR_COMMAND_IS_NOT_IN_RESERVED_RANGE = 3;
const unsigned int ERROR_COMMAND_IS_REGISTERED = 4;
const unsigned int ERROR_COMMAND_IS_NOT_REGISTERED = 5;
const unsigned int ERROR_END_OF_STRING_BYTE_NOT_IN_CHAR_ARRAY = 6;


class SimpleSerialProtocol : public Core {

public:
    typedef void (*CallbackPointer)();

    typedef void (*ErrorCallbackPointer)(unsigned int errorNum);


#ifdef SOFTWARESERIAL_SUPPORTED

    SimpleSerialProtocol(
            SoftwareSerial &softwareSerialRef,
            unsigned long baudrate,
            unsigned long waitForByteTimeout,
            ErrorCallbackPointer errorCallbackPointer,
            unsigned int commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
            unsigned int commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO);

    SimpleSerialProtocol(
            SoftwareSerial *softwareSerialPtr,
            unsigned long baudrate,
            unsigned long waitForByteTimeout,
            ErrorCallbackPointer errorCallbackPointer,
            unsigned int commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
            unsigned int commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO);

#endif

    SimpleSerialProtocol(
            HardwareSerial &hardwareSerialRef,
            unsigned long baudrate,
            unsigned long waitForByteTimeout,
            ErrorCallbackPointer errorCallbackPointer,
            unsigned int commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
            unsigned int commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    );

    SimpleSerialProtocol(
            HardwareSerial *hardwareSerialPtr,
            unsigned long baudrate,
            unsigned long waitForByteTimeout,
            ErrorCallbackPointer errorCallbackPointer,
            unsigned int commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
            unsigned int commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    );

    SimpleSerialProtocol(
            Stream &streamRef,
            unsigned int streamType,
            unsigned long baudrate,
            unsigned long waitForByteTimeout,
            ErrorCallbackPointer errorCallbackPointer,
            unsigned int commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
            unsigned int commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    );

    SimpleSerialProtocol(
            Stream *streamPtr,
            unsigned int streamType,
            unsigned long baudrate,
            unsigned long waitForByteTimeout,
            ErrorCallbackPointer errorCallbackPointer,
            unsigned int commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
            unsigned int commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    );

    ~SimpleSerialProtocol();

    void init();

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

    unsigned int commandCallbackRangeFrom;
    unsigned int commandCallbackRangeTo;

    bool _isInitialized = false;
    bool _isDead = false;

    bool isCommandRangeValid(); //
    bool isCommandInReservedRange(byte command); //
    bool isCommandRegistered(byte command); //

    void onWaitForByteTimeout();

    virtual void onGotCommandByte(byte command);

    void registerCommandCallback(byte command, CallbackPointer commandCallbackPointer); //

    unsigned int getCommandIndex(byte command); //
    void callCommandCallback(byte command);

    void error(unsigned int errorNum, bool dieInstantly); //
    void flushCommand();

private:

    ErrorCallbackPointer errorCallbackPointer = 0;
    CallbackPointer *commandCallbackPointers = 0;

    bool isWaitingForReadEot = false;

    void afterConstructor(
            ErrorCallbackPointer errorCallbackPointer,
            unsigned int commandCallbackRangeFrom,
            unsigned int commandCallbackRangeTo
    );

    void die();

};

#endif
