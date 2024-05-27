#ifndef SSP3_H
#define SSP3_H

#include <Arduino.h>

#include "callback/Callback.h"
#include "ssp_common.h"
#include "Core.h"

class Ssp3 final : public Core
{
public:
#ifdef SOFTWARESERIAL_SUPPORTED

    Ssp3(
        SoftwareSerial* softwareSerialPtr,
        const uint32_t baudrate,
        const uint32_t waitForByteTimeout,
        Callback* cbErrPtr = nullptr,
        const byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        const byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    ):
        Ssp3{
            true,
            softwareSerialPtr,
            baudrate,
            waitForByteTimeout,
            commandCallbackRangeFrom,
            commandCallbackRangeTo,
            cbErrPtr
        }
    {
    }

    Ssp3(
        SoftwareSerial* softwareSerialPtr,
        const uint32_t baudrate,
        const uint32_t waitForByteTimeout,
        const FunctionTypeStandaloneWithUint8Arg cbErrPtr = nullptr,
        const byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        const byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    ):
        Ssp3{
            softwareSerialPtr,
            baudrate,
            waitForByteTimeout,
            new Callback(cbErrPtr),
            commandCallbackRangeFrom,
            commandCallbackRangeTo,
        }
    {
    }

    template <typename T>
    Ssp3(
        SoftwareSerial* softwareSerialPtr,
        const uint32_t baudrate,
        const uint32_t waitForByteTimeout,
        const T* instance = nullptr,
        const FunctionTypeMemberWithUint8Arg<T> cbErrPtr = nullptr,
        const byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        const byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    ):
        Ssp3{
            softwareSerialPtr,
            baudrate,
            waitForByteTimeout,
            new Callback(instance, cbErrPtr),
            commandCallbackRangeFrom,
            commandCallbackRangeTo,
        }
    {
    }

    template <typename T>
    Ssp3(
        SoftwareSerial* softwareSerialPtr,
        const uint32_t baudrate,
        const uint32_t waitForByteTimeout,
        const T* instance = nullptr,
        const FunctionTypeConstMemberWithUint8Arg<T> cbErrPtr = nullptr,
        const byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        const byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    ):
        Ssp3{
            softwareSerialPtr,
            baudrate,
            waitForByteTimeout,
            new Callback(instance, cbErrPtr),
            commandCallbackRangeFrom,
            commandCallbackRangeTo,
        }
    {
    }

# endif //SOFTWARESERIAL_SUPPORTED

#ifdef HARDWARESERIAL

    Ssp3(
        HardwareSerial* hardwareSerialPtr,
        const uint32_t baudrate,
        const uint32_t waitForByteTimeout,
        Callback* cbErrPtr = nullptr,
        const byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        const byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    ):
        Ssp3{
            false,
            hardwareSerialPtr,
            baudrate,
            waitForByteTimeout,
            commandCallbackRangeFrom,
            commandCallbackRangeTo,
            cbErrPtr
        }
    {
    }

    Ssp3(
        HardwareSerial* hardwareSerialPtr,
        const uint32_t baudrate,
        const uint32_t waitForByteTimeout,
        const FunctionTypeStandaloneWithUint8Arg cbErrPtr = nullptr,
        const byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        const byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    ):
        Ssp3{
            hardwareSerialPtr,
            baudrate,
            waitForByteTimeout,
            new Callback(cbErrPtr),
            commandCallbackRangeFrom,
            commandCallbackRangeTo,
        }
    {
    }

    template <typename T>
    Ssp3(
        HardwareSerial* hardwareSerialPtr,
        const uint32_t baudrate,
        const uint32_t waitForByteTimeout,
        const T* instance = nullptr,
        const FunctionTypeMemberWithUint8Arg<T> cbErrPtr = nullptr,
        const byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        const byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    ):
        Ssp3{
            hardwareSerialPtr,
            baudrate,
            waitForByteTimeout,
            new Callback(instance, cbErrPtr),
            commandCallbackRangeFrom,
            commandCallbackRangeTo,
        }
    {
    }

    template <typename T>
    Ssp3(
        HardwareSerial* hardwareSerialPtr,
        const uint32_t baudrate,
        const uint32_t waitForByteTimeout,
        const T* instance = nullptr,
        const FunctionTypeConstMemberWithUint8Arg<T> cbErrPtr = nullptr,
        const byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        const byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    ):
        Ssp3{
            hardwareSerialPtr,
            baudrate,
            waitForByteTimeout,
            new Callback(instance, cbErrPtr),
            commandCallbackRangeFrom,
            commandCallbackRangeTo,
        }
    {
    }

# endif //HARDWARESERIAL

#ifdef USBAPISERIAL

    Ssp3(
        Serial_* usbapiSerialPtr,
        const uint32_t baudrate,
        const uint32_t waitForByteTimeout,
        Callback* cbErrPtr = nullptr,
        const byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        const byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    ):
        Ssp3{
            false,
            usbapiSerialPtr,
            baudrate,
            waitForByteTimeout,
            commandCallbackRangeFrom,
            commandCallbackRangeTo,
            cbErrPtr
        }
    {
    }

    Ssp3(
        Serial_* usbapiSerialPtr,
        const uint32_t baudrate,
        const uint32_t waitForByteTimeout,
        const FunctionTypeStandaloneWithUint8Arg cbErrPtr = nullptr,
        const byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        const byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    ):
        Ssp3{
            usbapiSerialPtr,
            baudrate,
            waitForByteTimeout,
            new Callback(cbErrPtr),
            commandCallbackRangeFrom,
            commandCallbackRangeTo,
        }
    {
    }

    template <typename T>
    Ssp3(
        Serial_* usbapiSerialPtr,
        const uint32_t baudrate,
        const uint32_t waitForByteTimeout,
        const T* instance = nullptr,
        const FunctionTypeMemberWithUint8Arg<T> cbErrPtr = nullptr,
        const byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        const byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    ):
        Ssp3{
            usbapiSerialPtr,
            baudrate,
            waitForByteTimeout,
            new Callback(instance, cbErrPtr),
            commandCallbackRangeFrom,
            commandCallbackRangeTo,
        }
    {
    }

    template <typename T>
    Ssp3(
        Serial_* usbapiSerialPtr,
        const uint32_t baudrate,
        const uint32_t waitForByteTimeout,
        const T* instance = nullptr,
        const FunctionTypeConstMemberWithUint8Arg<T> cbErrPtr = nullptr,
        const byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        const byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    ):
        Ssp3{
            usbapiSerialPtr,
            baudrate,
            waitForByteTimeout,
            new Callback(instance, cbErrPtr),
            commandCallbackRangeFrom,
            commandCallbackRangeTo,
        }
    {
    }

# endif //USBAPISERIAL

    virtual ~Ssp3(); // ensures proper cleanup when deleting derived objects via base class pointers.
    Ssp3(const Ssp3&) = delete; // forbid Copy-Ctor
    Ssp3& operator=(const Ssp3&) = delete; // forbid Copy-Asssignment

    void init() override;
    bool loop();

    void registerCommand(byte command, Callback* cbPtr);

    void registerCommand(const byte command, const FunctionTypeStandalone func)
    {
        this->registerCommand(command, new Callback{func});
    }

    void registerCommand(const byte command, const FunctionTypeStandaloneWithUint8Arg func)
    {
        this->registerCommand(command, new Callback{func});
    }

    template <typename T>
    void registerCommand(const byte command, const T* instance, FunctionTypeMember<T> func)
    {
        this->registerCommand(command, new Callback{instance, func});
    }

    template <typename T>
    void registerCommand(const byte command, const T* instance, FunctionTypeMemberWithUint8Arg<T> func)
    {
        this->registerCommand(command, new Callback{instance, func});
    }

    template <typename T>
    void registerCommand(const byte command, const T* instance, FunctionTypeConstMember<T> func)
    {
        this->registerCommand(command, new Callback{instance, func});
    }

    template <typename T>
    void registerCommand(const byte command, const T* instance, FunctionTypeConstMemberWithUint8Arg<T> func)
    {
        this->registerCommand(command, new Callback{instance, func});
    }

    void unregisterCommand(byte command);
    byte readCommand();
    void writeCommand(byte command) const;
    byte readEot();
    void writeEot() const;
    bool readCString(char* output, uint8_t maxLength) override;

protected:
    void _onWaitForByteTimeout() override;

private:
    Ssp3(
        bool isSoftwareSerial,
        Stream* streamPtr,
        uint32_t baudrate,
        uint32_t waitForByteTimeout,
        byte commandCallbackRangeFrom,
        byte commandCallbackRangeTo,
        Callback* cbErrPtr
    );

    byte _cbRangeFrom;
    byte _cbRangeTo;
    Callback* _cbErrPtr = nullptr;
    Callback** _cbPtrs = nullptr;
    bool _isInitialized = false;
    bool _isDead = false;
    bool _isWaitingForReadEot = false;

    bool _isCommandRangeValid() const;
    bool _isCommandInReservedRange(byte command) const;
    bool _isCommandRegistered(byte command) const;
    void _onGotCommandByte(byte command);
    uint16_t _getCommandIndex(byte command) const;
    void _callCommandCallback(byte command) const;
    void _error(uint8_t errorNum, bool dieImmediately);
    void _callErrorCallback(uint8_t errorNum) const;
    void _flushCommand();
    void _die();
};


#endif //SSP3_H
