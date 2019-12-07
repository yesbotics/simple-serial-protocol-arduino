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

const unsigned int EXTERNAL_COMMAND_CALLBACK_RANGE_FROM = 0;
const unsigned int EXTERNAL_COMMAND_CALLBACK_RANGE_TO = 255;

const unsigned int ERROR_IS_DEAD = 255;
const unsigned int ERROR_EOT_WAS_NOT_READ = 254;
const unsigned int ERROR_IS_NOT_WAITING_FOR_READ_EOT = 253;
const unsigned int ERROR_IS_NOT_EOT = 252;

const unsigned int ERROR_WAIT_FOR_BYTE_TIMEOUT = 0;
const unsigned int ERROR_IS_NOT_INITIALIZED = 1;
const unsigned int ERROR_EXTERNAL_COMMAND_RANGE_IS_INVALID = 2;
const unsigned int ERROR_EXTERNAL_COMMAND_IS_NOT_IN_RESERVED_RANGE = 3;
const unsigned int ERROR_EXTERNAL_COMMAND_IS_REGISTERED = 4;
const unsigned int ERROR_EXTERNAL_COMMAND_IS_NOT_REGISTERED = 5;


class SimpleSerialProtocol : public Core {

public:
    typedef void (*ExternalCallbackPointer)();

    typedef void (*ExternalFatalErrorCallbackPointer)(unsigned int errorNum);


#ifdef SOFTWARESERIAL_SUPPORTED

    SimpleSerialProtocol(
            SoftwareSerial &softwareSerialRef,
            unsigned long baudrate,
            unsigned long waitForByteTimeout,
            ExternalFatalErrorCallbackPointer externalFatalErrorCallbackPointer,
            unsigned int externalCommandCallbackRangeFrom = EXTERNAL_COMMAND_CALLBACK_RANGE_FROM,
            unsigned int externalCommandCallbackRangeTo = EXTERNAL_COMMAND_CALLBACK_RANGE_TO);

    SimpleSerialProtocol(
            SoftwareSerial *softwareSerialPtr,
            unsigned long baudrate,
            unsigned long waitForByteTimeout,
            ExternalFatalErrorCallbackPointer externalFatalErrorCallbackPointer,
            unsigned int externalCommandCallbackRangeFrom = EXTERNAL_COMMAND_CALLBACK_RANGE_FROM,
            unsigned int externalCommandCallbackRangeTo = EXTERNAL_COMMAND_CALLBACK_RANGE_TO);

#endif

    SimpleSerialProtocol(
            HardwareSerial &hardwareSerialRef,
            unsigned long baudrate,
            unsigned long waitForByteTimeout,
            ExternalFatalErrorCallbackPointer externalFatalErrorCallbackPointer,
            unsigned int externalCommandCallbackRangeFrom = EXTERNAL_COMMAND_CALLBACK_RANGE_FROM,
            unsigned int externalCommandCallbackRangeTo = EXTERNAL_COMMAND_CALLBACK_RANGE_TO
    );

    SimpleSerialProtocol(
            HardwareSerial *hardwareSerialPtr,
            unsigned long baudrate,
            unsigned long waitForByteTimeout,
            ExternalFatalErrorCallbackPointer externalFatalErrorCallbackPointer,
            unsigned int externalCommandCallbackRangeFrom = EXTERNAL_COMMAND_CALLBACK_RANGE_FROM,
            unsigned int externalCommandCallbackRangeTo = EXTERNAL_COMMAND_CALLBACK_RANGE_TO
    );

    SimpleSerialProtocol(
            Stream &streamRef,
            unsigned int streamType,
            unsigned long baudrate,
            unsigned long waitForByteTimeout,
            ExternalFatalErrorCallbackPointer externalFatalErrorCallbackPointer,
            unsigned int externalCommandCallbackRangeFrom = EXTERNAL_COMMAND_CALLBACK_RANGE_FROM,
            unsigned int externalCommandCallbackRangeTo = EXTERNAL_COMMAND_CALLBACK_RANGE_TO
    );

    SimpleSerialProtocol(
            Stream *streamPtr,
            unsigned int streamType,
            unsigned long baudrate,
            unsigned long waitForByteTimeout,
            ExternalFatalErrorCallbackPointer externalFatalErrorCallbackPointer,
            unsigned int externalCommandCallbackRangeFrom = EXTERNAL_COMMAND_CALLBACK_RANGE_FROM,
            unsigned int externalCommandCallbackRangeTo = EXTERNAL_COMMAND_CALLBACK_RANGE_TO
    );

    ~SimpleSerialProtocol();

    void init();

    void registerCommand(byte command, ExternalCallbackPointer externalCommandCallbackPointer);

    //overrideable
    virtual bool loop();

    byte readCommand();

    byte readEot();

    void writeCommand(byte command);

    void writeEot();

protected:

    unsigned int externalCommandCallbackRangeFrom;
    unsigned int externalCommandCallbackRangeTo;

    bool _isInitialized = false;
    bool _isDead = false;

    bool isExternalCommandRangeValid(); //
    bool isExternalCommandInReservedRange(byte command); //
    bool isExternalCommandRegistered(byte command); //

    void onWaitForByteTimeout();

    virtual void onGotCommandByte(byte command);

    void registerExternalCommandCallback(byte command, ExternalCallbackPointer externalCommandCallbackPointer); //

    unsigned int getExternalCommandIndex(byte command); //
    void callExternalCommandCallback(byte command);

    void fatalError(unsigned int errorNum, bool dieInstantly); //
    void flushCommand();

private:

    ExternalFatalErrorCallbackPointer externalFatalErrorCallbackPointer = 0;
    ExternalCallbackPointer *externalCommandCallbackPointers = 0;

    bool isWaitingForReadEot = false;

    void afterConstructor(
            ExternalFatalErrorCallbackPointer externalFatalErrorCallbackPointer,
            unsigned int externalCommandCallbackRangeFrom,
            unsigned int externalCommandCallbackRangeTo
    );

    void die();

};

#endif
