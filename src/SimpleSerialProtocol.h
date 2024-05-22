/*
 *  Created on: 02.02.2019
 *      Author: Albrecht Nitsche und Jens Kabisch
 */

#ifndef SIMPLE_SERIAL_PROTOCOL_H
#define SIMPLE_SERIAL_PROTOCOL_H

#include "Core.h"

constexpr unsigned long FLUSH_TIMEOUT = 1000;
constexpr unsigned long COMMAND_MESSAGE_COMPLETE_TIMEOUT = 1000;

constexpr byte CHAR_EOT = 0x0A; // 10 // End of Transmission - Line Feed Zeichen \n

constexpr byte COMMAND_CALLBACK_RANGE_FROM = 0;
constexpr byte COMMAND_CALLBACK_RANGE_TO = 255;

constexpr uint8_t ERROR_IS_DEAD = 255;
constexpr uint8_t ERROR_EOT_WAS_NOT_READ = 254;
constexpr uint8_t ERROR_IS_NOT_WAITING_FOR_READ_EOT = 253;
constexpr uint8_t ERROR_IS_NOT_EOT = 252;

constexpr uint8_t ERROR_WAIT_FOR_BYTE_TIMEOUT = 0;
constexpr uint8_t ERROR_IS_NOT_INITIALIZED = 1;
constexpr uint8_t ERROR_COMMAND_RANGE_IS_INVALID = 2;
constexpr uint8_t ERROR_COMMAND_IS_NOT_IN_RESERVED_RANGE = 3;
constexpr uint8_t ERROR_COMMAND_IS_REGISTERED = 4;
constexpr uint8_t ERROR_COMMAND_IS_NOT_REGISTERED = 5;
constexpr uint8_t ERROR_END_OF_STRING_BYTE_NOT_IN_CHAR_ARRAY = 6;


class SimpleSerialProtocol final : public Core
{
public:
    using StandaloneCallbackPointer = void (*)();
    using StandaloneErrorCallbackPointer = void (*)(uint8_t errorNum);
    template <typename T>
    using MemberCallbackPointer = void (T::*)();
    template <typename T>
    using MemberErrorCallbackPointer = void (T::*)(uint8_t errorNum);


#ifdef SOFTWARESERIAL_SUPPORTED

    SimpleSerialProtocol(
        SoftwareSerial& softwareSerialRef,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        StandaloneErrorCallbackPointer errorCallbackPointer = nullptr,
        byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO);

    SimpleSerialProtocol(
        SoftwareSerial* softwareSerialPtr,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        StandaloneErrorCallbackPointer errorCallbackPointer = nullptr,
        byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO);

#endif

#ifdef HARDWARESERIAL
    SimpleSerialProtocol(
        HardwareSerial& hardwareSerialRef,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        StandaloneErrorCallbackPointer errorCallbackPointer = nullptr,
        byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    );

    SimpleSerialProtocol(
        HardwareSerial* hardwareSerialPtr,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        StandaloneErrorCallbackPointer errorCallbackPointer = nullptr,
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
    void setDieInstantlyOnNotRegisteredCommand(bool die);
    void registerCommand(byte command, StandaloneCallbackPointer commandCallbackPointer);
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
    void _registerCommandCallback(byte command, StandaloneCallbackPointer commandCallbackPointer) const;
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
        StandaloneErrorCallbackPointer errorCallbackPointer,
        byte commandCallbackRangeFrom,
        byte commandCallbackRangeTo
    );
    SimpleSerialProtocol(
        Stream* streamPtr,
        bool isSoftwareSerial,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        StandaloneErrorCallbackPointer errorCallbackPointer,
        byte commandCallbackRangeFrom,
        byte commandCallbackRangeTo
    );

    StandaloneErrorCallbackPointer _standaloneErrorCallbackPointer = nullptr;
    StandaloneCallbackPointer* _standaloneCommandCallbackPointers = nullptr;
    // MemberErrorCallbackPointer* commandCallbackPointers = nullptr;
    // MemberCallbackPointer* commandCallbackPointers = nullptr;

    bool _dieImmediatelyOnNotRegisteredCommand = true;
    bool _isWaitingForReadEot = false;
    void _afterConstructor();
    void _die();
};

#endif
