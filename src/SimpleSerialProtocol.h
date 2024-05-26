/*
 *  Created on: 02.02.2019
 *      Author: Albrecht Nitsche und Jens Kabisch
 */

#ifndef SIMPLE_SERIAL_PROTOCOL_H
#define SIMPLE_SERIAL_PROTOCOL_H

#include "Core.h"
#include "ssp_common.h"

class SimpleSerialProtocol final : public Core
{
public:
    using CallbackPointer = void (*)();
    using ErrorCallbackPointer = void (*)(uint8_t errorNum);


#ifdef SOFTWARESERIAL_SUPPORTED

    SimpleSerialProtocol(
        SoftwareSerial& softwareSerialRef,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        ErrorCallbackPointer errorCallbackPointer = nullptr,
        byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO);

    SimpleSerialProtocol(
        SoftwareSerial* softwareSerialPtr,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        ErrorCallbackPointer errorCallbackPointer = nullptr,
        byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO);

#endif

#ifdef HARDWARESERIAL

    SimpleSerialProtocol(
        HardwareSerial& hardwareSerialRef,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        ErrorCallbackPointer errorCallbackPointer = nullptr,
        byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    );

    SimpleSerialProtocol(
        HardwareSerial* hardwareSerialPtr,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        ErrorCallbackPointer errorCallbackPointer = nullptr,
        byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    );
#endif

#ifdef USBAPISERIAL
    SimpleSerialProtocol(
        Serial_& usbapiSerialRef,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        ErrorCallbackPointer errorCallbackPointer = nullptr,
        byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    );

    SimpleSerialProtocol(
        Serial_* usbapiSerialPtr,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        ErrorCallbackPointer errorCallbackPointer = nullptr,
        byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    );
#endif

    virtual ~SimpleSerialProtocol();
    SimpleSerialProtocol(const SimpleSerialProtocol&) = delete; // verbiete Copy-Ctor
    SimpleSerialProtocol& operator=(const SimpleSerialProtocol&) = delete; // verbiete Copy-Asssignment

    void init() override;
    void setDieImmediatelyOnNotRegisteredCommand(bool die);
    void registerCommand(byte command, CallbackPointer commandCallbackPointer);
    void unregisterCommand(byte command);
    bool loop();
    byte readCommand();
    byte readEot();
    void writeCommand(byte command) const;
    void writeEot() const;
    bool readCString(char* output, uint8_t maxLength) override;

protected:
    byte _commandCallbackRangeFrom;
    byte _commandCallbackRangeTo;
    bool _isInitialized = false;
    bool _isDead = false;
    bool _isCommandRangeValid() const;
    bool _isCommandInReservedRange(byte command) const;
    bool _isCommandRegistered(byte command) const;
    void _onWaitForByteTimeout() override;
    void _onGotCommandByte(byte command);
    void _registerCommandCallback(byte command, CallbackPointer commandCallbackPointer) const;
    void _unregisterCommandCallback(byte command) const;
    uint8_t _getCommandIndex(byte command) const;
    void _callCommandCallback(byte command) const;
    void _error(uint8_t errorNum, bool dieImmediately);
    void _flushCommand();

private:
    SimpleSerialProtocol(
        Stream& streamRef,
        bool isSoftwareSerial,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        ErrorCallbackPointer errorCallbackPointer,
        byte commandCallbackRangeFrom,
        byte commandCallbackRangeTo
    );
    SimpleSerialProtocol(
        Stream* streamPtr,
        bool isSoftwareSerial,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        ErrorCallbackPointer errorCallbackPointer,
        byte commandCallbackRangeFrom,
        byte commandCallbackRangeTo
    );

    ErrorCallbackPointer _errorCallbackPointer = nullptr;
    CallbackPointer* _commandCallbackPointers = nullptr;

    bool _dieImmediatelyOnNotRegisteredCommand = true;
    bool _isWaitingForReadEot = false;
    void _afterConstructor();
    void _die();
};

#endif
