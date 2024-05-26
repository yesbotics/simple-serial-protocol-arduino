/*
 *  Created on: 02.02.2019
 *      Author: Albrecht Nitsche und Jens Kabisch
 */

#ifndef SIMPLE_SERIAL_PROTOCOL_H
#define SIMPLE_SERIAL_PROTOCOL_H

#include "common.h"
#include "Core.h"


class SimpleSerialProtocol final : public Core
{
public:
    using StandaloneCallbackPointer = void (*)();
    using StandaloneErrorCallbackPointer = void (*)(uint8_t);
    template <typename T>
    using MemberErrorCallbackPointer = void (T::*)(uint8_t) const;
    template <typename T>
    using MemberCallbackPointer = void (T::*)() const;


#ifdef SOFTWARESERIAL_SUPPORTED

    SimpleSerialProtocol(
        SoftwareSerial& softwareSerialRef,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        StandaloneErrorCallbackPointer standaloneErrorCallbackPointer = nullptr,
        byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO);

    SimpleSerialProtocol(
        SoftwareSerial* softwareSerialPtr,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        StandaloneErrorCallbackPointer standaloneErrorCallbackPointer = nullptr,
        byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO);


    template <typename T>
    SimpleSerialProtocol(
        SoftwareSerial& softwareSerialRef,
        const unsigned long baudrate,
        const unsigned long waitForByteTimeout,
        T* instanceOfMemberErrorCallbackPointer,
        MemberErrorCallbackPointer<T> memberErrorCallbackPointer,
        const byte commandCallbackRangeFrom,
        const byte commandCallbackRangeTo) :
        SimpleSerialProtocol{
            static_cast<Stream&>(softwareSerialRef),
            true,
            baudrate,
            waitForByteTimeout,
            instanceOfMemberErrorCallbackPointer,
            memberErrorCallbackPointer,
            commandCallbackRangeFrom,
            commandCallbackRangeTo
        }
    {
    }

    template <typename T>
    SimpleSerialProtocol(
        SoftwareSerial* softwareSerialPtr,
        const unsigned long baudrate,
        const unsigned long waitForByteTimeout,
        T* instanceOfMemberErrorCallbackPointer,
        MemberErrorCallbackPointer<T> memberErrorCallbackPointer,
        const byte commandCallbackRangeFrom,
        const byte commandCallbackRangeTo) :
        SimpleSerialProtocol{
            static_cast<Stream*>(softwareSerialPtr),
            true,
            baudrate,
            waitForByteTimeout,
            instanceOfMemberErrorCallbackPointer,
            memberErrorCallbackPointer,
            commandCallbackRangeFrom,
            commandCallbackRangeTo
        }
    {
    }

#endif

#ifdef HARDWARESERIAL

    SimpleSerialProtocol(
        HardwareSerial& hardwareSerialRef,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        StandaloneErrorCallbackPointer standaloneErrorCallbackPointer = nullptr,
        byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    );

    SimpleSerialProtocol(
        HardwareSerial* hardwareSerialPtr,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        StandaloneErrorCallbackPointer standaloneErrorCallbackPointer = nullptr,
        byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    );


    template <typename T>
    SimpleSerialProtocol(
        HardwareSerial& hardwareSerialRef,
        const unsigned long baudrate,
        const unsigned long waitForByteTimeout,
        T* instanceOfMemberErrorCallbackPointer,
        MemberErrorCallbackPointer<T> memberErrorCallbackPointer,
        const byte commandCallbackRangeFrom,
        const byte commandCallbackRangeTo) :
        SimpleSerialProtocol{
            static_cast<Stream&>(hardwareSerialRef),
            false,
            baudrate,
            waitForByteTimeout,
            instanceOfMemberErrorCallbackPointer,
            memberErrorCallbackPointer,
            commandCallbackRangeFrom,
            commandCallbackRangeTo
        }
    {
    }

    template <typename T>
    SimpleSerialProtocol(
        HardwareSerial* hardwareSerialPtr,
        const unsigned long baudrate,
        const unsigned long waitForByteTimeout,
        T* instanceOfMemberErrorCallbackPointer,
        MemberErrorCallbackPointer<T> memberErrorCallbackPointer,
        const byte commandCallbackRangeFrom,
        const byte commandCallbackRangeTo) :
        SimpleSerialProtocol{
            static_cast<Stream*>(hardwareSerialPtr),
            false,
            baudrate,
            waitForByteTimeout,
            instanceOfMemberErrorCallbackPointer,
            memberErrorCallbackPointer,
            commandCallbackRangeFrom,
            commandCallbackRangeTo
        }
    {
    }

#endif

#ifdef USBAPISERIAL

    SimpleSerialProtocol(
        Serial_& usbapiSerialRef,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        StandaloneErrorCallbackPointer standaloneErrorCallbackPointer = nullptr,
        byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    );

    SimpleSerialProtocol(
        Serial_* usbapiSerialPtr,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        StandaloneErrorCallbackPointer standaloneErrorCallbackPointer = nullptr,
        byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    );

    template <typename T>
    SimpleSerialProtocol(
        Serial_& usbapiSerialRef,
        const unsigned long baudrate,
        const unsigned long waitForByteTimeout,
        T* instanceOfMemberErrorCallbackPointer,
        MemberErrorCallbackPointer<T> memberErrorCallbackPointer,
        const byte commandCallbackRangeFrom,
        const byte commandCallbackRangeTo) :
        SimpleSerialProtocol{
            static_cast<Stream&>(usbapiSerialRef),
            false,
            baudrate,
            waitForByteTimeout,
            instanceOfMemberErrorCallbackPointer,
            memberErrorCallbackPointer,
            commandCallbackRangeFrom,
            commandCallbackRangeTo
        }
    {
    }

    template <typename T>
    SimpleSerialProtocol(
        Serial_* usbapiSerialPtr,
        const unsigned long baudrate,
        const unsigned long waitForByteTimeout,
        T* instanceOfMemberErrorCallbackPointer,
        MemberErrorCallbackPointer<T> memberErrorCallbackPointer,
        const byte commandCallbackRangeFrom,
        const byte commandCallbackRangeTo) :
        SimpleSerialProtocol{
            static_cast<Stream*>(usbapiSerialPtr),
            false,
            baudrate,
            waitForByteTimeout,
            instanceOfMemberErrorCallbackPointer,
            memberErrorCallbackPointer,
            commandCallbackRangeFrom,
            commandCallbackRangeTo
        }
    {
    }

#endif

    virtual ~SimpleSerialProtocol();
    SimpleSerialProtocol(const SimpleSerialProtocol&) = delete; // verbiete Copy-Ctor
    SimpleSerialProtocol& operator=(const SimpleSerialProtocol&) = delete; // verbiete Copy-Asssignment

    void init() override;
    void setDieImmediatelyOnNotRegisteredCommand(bool die);
    void registerCommand(byte command, StandaloneCallbackPointer standaloneCommandCallbackPointer);
    void unregisterCommand(byte command);
    bool loop();
    byte readCommand();
    byte readEot();
    void writeCommand(byte command) const;
    void writeEot() const;
    bool readCString(char* output, uint8_t maxLength) override;


    template <typename T>
    void registerCommand(
        const byte command,
        const T* instanceOfMemberCallackPointer,
        const MemberCallbackPointer<T> memberCommandCallbackPointer
    )
    {
        if (!this->_registerCommandPrecheck(command)) return;
        const uint8_t commandIndex = this->_getCommandIndex(command);
        this->_instancesOfMemberCallbackPointers[commandIndex]
            = static_cast<VoidPointer>(const_cast<T*>(instanceOfMemberCallackPointer));
        this->_memberCommandCallbackPointers[commandIndex]
            = reinterpret_cast<SspMemberCallbackPointer>(memberCommandCallbackPointer);
    }

    template <typename T>
    void unregisterCommand(const byte command)
    {
        if (!this->_unregisterCommandPrecheck(command)) return;
        const uint8_t commandIndex = this->_getCommandIndex(command);
        this->_instancesOfMemberCallbackPointers[commandIndex] = nullptr;
        this->_memberCommandCallbackPointers[commandIndex] = nullptr;
    }

protected:
    void _onWaitForByteTimeout() override;

private:
    using SspMemberErrorCallbackPointer = void (SimpleSerialProtocol::*)(uint8_t) const;
    using SspMemberCallbackPointer = void (SimpleSerialProtocol::*)() const;
    using SspPointer = SimpleSerialProtocol*;
    using VoidPointer = void*;

    byte _commandCallbackRangeFrom;
    byte _commandCallbackRangeTo;
    bool _isInitialized = false;
    bool _isDead = false;
    bool _dieImmediatelyOnNotRegisteredCommand = true;
    bool _isWaitingForReadEot = false;

    StandaloneErrorCallbackPointer _standaloneErrorCallbackPointer = nullptr;
    VoidPointer _instanceOfMemberErrorCallbackPointer = nullptr;
    SspMemberCallbackPointer _memberErrorCallbackPointer = nullptr;

    StandaloneCallbackPointer* _standaloneCommandCallbackPointers = nullptr;
    VoidPointer* _instancesOfMemberCallbackPointers = nullptr;
    SspMemberCallbackPointer* _memberCommandCallbackPointers = nullptr;

    bool _registerCommandPrecheck(byte command);
    bool _unregisterCommandPrecheck(byte command);

    bool _isCommandRangeValid() const;
    bool _isCommandInReservedRange(byte command) const;
    bool _isCommandRegistered(byte command) const;
    void _onGotCommandByte(byte command);
    uint8_t _getCommandIndex(byte command) const;
    void _callCommandCallback(byte command) const;
    void _error(uint8_t errorNum, bool dieImmediately);
    void _callErrorCallback(uint8_t errorNum) const;
    void _flushCommand();
    void _afterConstructor();
    void _die();

    SimpleSerialProtocol(
        Stream& streamRef,
        bool isSoftwareSerial,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        StandaloneErrorCallbackPointer standaloneErrorCallbackPointer,
        byte commandCallbackRangeFrom,
        byte commandCallbackRangeTo
    );
    SimpleSerialProtocol(
        Stream* streamPtr,
        bool isSoftwareSerial,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        StandaloneErrorCallbackPointer standaloneErrorCallbackPointer,
        byte commandCallbackRangeFrom,
        byte commandCallbackRangeTo
    );

    template <typename T>
    SimpleSerialProtocol(
        Stream& streamRef,
        const bool isSoftwareSerial,
        const unsigned long baudrate,
        const unsigned long waitForByteTimeout,
        T* instanceOfMemberErrorCallbackPointer,
        MemberErrorCallbackPointer<T> memberErrorCallbackPointer,
        const byte commandCallbackRangeFrom,
        const byte commandCallbackRangeTo):
        Core{
            streamRef,
            isSoftwareSerial,
            baudrate,
            waitForByteTimeout
        },
        _isStandaloneCallbackSsp{false},
        _commandCallbackRangeFrom{commandCallbackRangeFrom},
        _commandCallbackRangeTo{commandCallbackRangeTo},
        _instanceOfMemberErrorCallbackPointer{static_cast<VoidPointer>(instanceOfMemberErrorCallbackPointer)},
        _memberErrorCallbackPointer{reinterpret_cast<SspMemberCallbackPointer>(memberErrorCallbackPointer)}
    {
        _afterConstructor();
    }

    template <typename T>
    SimpleSerialProtocol(
        Stream* streamPtr,
        const bool isSoftwareSerial,
        const unsigned long baudrate,
        const unsigned long waitForByteTimeout,
        T* instanceOfMemberErrorCallbackPointer,
        MemberErrorCallbackPointer<T> memberErrorCallbackPointer,
        const byte commandCallbackRangeFrom,
        const byte commandCallbackRangeTo):
        Core{
            streamPtr,
            isSoftwareSerial,
            baudrate,
            waitForByteTimeout
        },
        _isStandaloneCallbackSsp{false},
        _commandCallbackRangeFrom{commandCallbackRangeFrom},
        _commandCallbackRangeTo{commandCallbackRangeTo},
        _instanceOfMemberErrorCallbackPointer{static_cast<VoidPointer>(instanceOfMemberErrorCallbackPointer)},
        _memberErrorCallbackPointer{reinterpret_cast<SspMemberCallbackPointer>(memberErrorCallbackPointer)}
    {
        _afterConstructor();
    }
};

#endif
