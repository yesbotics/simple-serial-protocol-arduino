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
        uint32_t baudrate,
        uint32_t waitForByteTimeout,
        Callback* cbErrPtr = nullptr,
        byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    );
# endif //SOFTWARESERIAL_SUPPORTED
#ifdef HARDWARESERIAL
    Ssp3(
        HardwareSerial* hardwareSerialPtr,
        uint32_t baudrate,
        uint32_t waitForByteTimeout,
        Callback* cbErrPtr = nullptr,
        byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    );
# endif //HARDWARESERIAL
#ifdef USBAPISERIAL
    Ssp3(
        Serial_* usbapiSerialPtr,
        uint32_t baudrate,
        uint32_t waitForByteTimeout,
        Callback* cbErrPtr = nullptr,
        byte commandCallbackRangeFrom = COMMAND_CALLBACK_RANGE_FROM,
        byte commandCallbackRangeTo = COMMAND_CALLBACK_RANGE_TO
    );
# endif //USBAPISERIAL

    virtual ~Ssp3(); // ensures proper cleanup when deleting derived objects via base class pointers.
    Ssp3(const Ssp3&) = delete; // forbid Copy-Ctor
    Ssp3& operator=(const Ssp3&) = delete; // forbid Copy-Asssignment

    void init() override;
    bool loop();

    void registerCommand(byte command, Callback* cbPtr);
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
