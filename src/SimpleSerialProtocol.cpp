#include "SimpleSerialProtocol.h"

/***************************** PUBLIC *********************************/

#ifdef SOFTWARESERIAL_SUPPORTED
SimpleSerialProtocol::SimpleSerialProtocol(
    SoftwareSerial& softwareSerialRef,
    const unsigned long baudrate,
    const unsigned long waitForByteTimeout,
    const ErrorCallbackPointer errorCallbackPointer,
    const byte commandCallbackRangeFrom,
    const byte commandCallbackRangeTo) :
    SimpleSerialProtocol{
        static_cast<Stream&>(softwareSerialRef),
        true,
        baudrate,
        waitForByteTimeout,
        errorCallbackPointer,
        commandCallbackRangeFrom,
        commandCallbackRangeTo
    }
{
}

SimpleSerialProtocol::SimpleSerialProtocol(
    SoftwareSerial* softwareSerialPtr,
    const unsigned long baudrate,
    const unsigned long waitForByteTimeout,
    const ErrorCallbackPointer errorCallbackPointer,
    const byte commandCallbackRangeFrom,
    const byte commandCallbackRangeTo) :
    SimpleSerialProtocol{
        static_cast<Stream*>(softwareSerialPtr),
        true,
        baudrate,
        waitForByteTimeout,
        errorCallbackPointer,
        commandCallbackRangeFrom,
        commandCallbackRangeTo
    }
{
}
#endif


#ifdef HARDWARESERIAL
SimpleSerialProtocol::SimpleSerialProtocol(
    HardwareSerial& hardwareSerialRef,
    const unsigned long baudrate,
    const unsigned long waitForByteTimeout,
    const ErrorCallbackPointer errorCallbackPointer,
    const byte commandCallbackRangeFrom,
    const byte commandCallbackRangeTo) :
    SimpleSerialProtocol{
        static_cast<Stream&>(hardwareSerialRef),
        false,
        baudrate,
        waitForByteTimeout,
        errorCallbackPointer,
        commandCallbackRangeFrom,
        commandCallbackRangeTo
    }
{
}

SimpleSerialProtocol::SimpleSerialProtocol(
    HardwareSerial* hardwareSerialPtr,
    const unsigned long baudrate,
    const unsigned long waitForByteTimeout,
    const ErrorCallbackPointer errorCallbackPointer,
    const byte commandCallbackRangeFrom,
    const byte commandCallbackRangeTo) :
    SimpleSerialProtocol{
        static_cast<Stream*>(hardwareSerialPtr),
        false,
        baudrate,
        waitForByteTimeout,
        errorCallbackPointer,
        commandCallbackRangeFrom,
        commandCallbackRangeTo
    }
{
}
#endif

#ifdef USBAPISERIAL
SimpleSerialProtocol::SimpleSerialProtocol(
    Serial_& usbapiSerialRef,
    const unsigned long baudrate,
    const unsigned long waitForByteTimeout,
    const ErrorCallbackPointer _errorCallbackPointer,
    const byte _commandCallbackRangeFrom,
    const byte _commandCallbackRangeTo) :
    SimpleSerialProtocol{
        static_cast<Stream&>(usbapiSerialRef),
        false,
        baudrate,
        waitForByteTimeout,
        _errorCallbackPointer,
        _commandCallbackRangeFrom,
        _commandCallbackRangeTo
    }
{
}

SimpleSerialProtocol::SimpleSerialProtocol(
    Serial_* usbapiSerialPtr,
    const unsigned long baudrate,
    const unsigned long waitForByteTimeout,
    const ErrorCallbackPointer _errorCallbackPointer,
    const byte _commandCallbackRangeFrom,
    const byte _commandCallbackRangeTo) :
    SimpleSerialProtocol{
        static_cast<Stream*>(usbapiSerialPtr),
        false,
        baudrate,
        waitForByteTimeout,
        _errorCallbackPointer,
        _commandCallbackRangeFrom,
        _commandCallbackRangeTo
    }
{
}
#endif


SimpleSerialProtocol::SimpleSerialProtocol(
    Stream& streamRef,
    const bool isSoftwareSerial,
    const unsigned long baudrate,
    const unsigned long waitForByteTimeout,
    const ErrorCallbackPointer errorCallbackPointer,
    const byte commandCallbackRangeFrom,
    const byte commandCallbackRangeTo
) :
    Core{
        streamRef,
        isSoftwareSerial,
        baudrate,
        waitForByteTimeout
    },
    _commandCallbackRangeFrom{commandCallbackRangeFrom},
    _commandCallbackRangeTo{commandCallbackRangeTo},
    _errorCallbackPointer{errorCallbackPointer}
{
    _afterConstructor();
}


SimpleSerialProtocol::SimpleSerialProtocol(
    Stream* streamPtr,
    const bool isSoftwareSerial,
    const unsigned long baudrate,
    const unsigned long waitForByteTimeout,
    const ErrorCallbackPointer errorCallbackPointer,
    const byte commandCallbackRangeFrom,
    const byte commandCallbackRangeTo) :
    Core{
        streamPtr,
        isSoftwareSerial,
        baudrate,
        waitForByteTimeout
    },
    _commandCallbackRangeFrom{commandCallbackRangeFrom},
    _commandCallbackRangeTo{commandCallbackRangeTo},
    _errorCallbackPointer{errorCallbackPointer}
{
    _afterConstructor();
}

SimpleSerialProtocol::~SimpleSerialProtocol()
{
    delete[] this->_commandCallbackPointers;
}

void SimpleSerialProtocol::_afterConstructor()
{
    const byte commandCallbackPointerBufferMaxIndex = abs(_commandCallbackRangeTo - _commandCallbackRangeFrom);
    const uint16_t commandCallbackPointerBufferSize = static_cast<uint16_t>(commandCallbackPointerBufferMaxIndex) + 1;
    this->_commandCallbackPointers = new CallbackPointer[commandCallbackPointerBufferSize];
    for (uint16_t i = 0; i <= commandCallbackPointerBufferMaxIndex; i++)
    {
        this->_commandCallbackPointers[i] = nullptr;
    }
}

void SimpleSerialProtocol::init()
{
    Core::init();
    //    Serial.println(F("SimpleSerialProtocol::init"));
    if (!this->_isCommandRangeValid())
    {
        this->_error(ERROR_COMMAND_RANGE_IS_INVALID, true);
        return;
    }
    this->_isInitialized = true;
}

bool SimpleSerialProtocol::loop()
{
    //	Serial.print(F("SimpleSerialProtocol::loop"));

    if (this->_isDead)
    {
        return false;
    }

    if (this->_isWaitingForReadEot)
    {
        this->_error(ERROR_EOT_WAS_NOT_READ, true);
        return false;
    }

    if (this->streamPointer->available() > 0)
    {
        const byte command = this->readCommand();
        this->_onGotCommandByte(command);
    }

    return true;
}

byte SimpleSerialProtocol::readCommand()
{
    const byte bite = this->readByte();
    this->_isWaitingForReadEot = true;
    return bite;
}

byte SimpleSerialProtocol::readEot()
{
    //TODO: check, ob eot ausgelesen werden darf, nur wenn this->_isWaitingForReadEot true ist

    if (!this->_isWaitingForReadEot)
    {
        this->_error(ERROR_IS_NOT_WAITING_FOR_READ_EOT, true);
        return CHAR_EOT;
    }

    this->_isWaitingForReadEot = false;
    const byte bite = this->readByte();
    if (bite != CHAR_EOT)
    {
        this->_error(ERROR_IS_NOT_EOT, true);
    }
    return bite;
}

void SimpleSerialProtocol::writeCommand(const byte command) const
{
    this->writeByte(command);
}

void SimpleSerialProtocol::writeEot() const
{
    this->writeByte(CHAR_EOT);
}

void SimpleSerialProtocol::setDieImmediatelyOnNotRegisteredCommand(const bool die)
{
    this->_dieImmediatelyOnNotRegisteredCommand = die;
}

void SimpleSerialProtocol::registerCommand(const byte command, const CallbackPointer commandCallbackPointer)
{
    //	Serial.println(F("SimpleSerialProtocol::registerCommand"));

    if (!this->_isInitialized)
    {
        this->_error(ERROR_IS_NOT_INITIALIZED, true);
        return;
    }

    if (!this->_isCommandInReservedRange(command))
    {
        this->_error(ERROR_COMMAND_IS_NOT_IN_RESERVED_RANGE, true);
        return;
    }

    if (this->_isCommandRegistered(command))
    {
        this->_error(ERROR_COMMAND_IS_REGISTERED, true);
        return;
    }

    this->_registerCommandCallback(command, commandCallbackPointer);
}

void SimpleSerialProtocol::unregisterCommand(const byte command)
{
    if (!this->_isInitialized)
    {
        this->_error(ERROR_IS_NOT_INITIALIZED, true);
        return;
    }

    if (!this->_isCommandInReservedRange(command))
    {
        this->_error(ERROR_COMMAND_IS_NOT_IN_RESERVED_RANGE, true);
        return;
    }

    if (!this->_isCommandRegistered(command))
    {
        this->_error(ERROR_COMMAND_IS_NOT_REGISTERED, true);
        return;
    }

    this->_unregisterCommandCallback(command);
}

bool SimpleSerialProtocol::readCString(char* output, const uint8_t maxLength)
{
    const bool successful = Core::readCString(output, maxLength);
    if (!successful)
    {
        this->_error(ERROR_END_OF_STRING_BYTE_NOT_IN_CHAR_ARRAY, true);
    }
    return successful;
}

/***************************** PROTECTED *********************************/

void SimpleSerialProtocol::_onWaitForByteTimeout()
{
    this->_error(ERROR_WAIT_FOR_BYTE_TIMEOUT, true);
}

void SimpleSerialProtocol::_onGotCommandByte(const byte command)
{
    if (!this->_isInitialized)
    {
        this->_error(ERROR_IS_NOT_INITIALIZED, true);
        return;
    }

    if (!this->_isCommandInReservedRange(command))
    {
        this->_error(ERROR_COMMAND_IS_NOT_IN_RESERVED_RANGE, true);
        this->_flushCommand();
        return;
    }

    if (!this->_isCommandRegistered(command))
    {
        this->_error(ERROR_COMMAND_IS_NOT_REGISTERED, _dieImmediatelyOnNotRegisteredCommand);
        this->_flushCommand();
        return;
    }

    this->_callCommandCallback(command);
}

void SimpleSerialProtocol::_registerCommandCallback(
    const byte command,
    const CallbackPointer commandCallbackPointer
) const
{
    const uint8_t commandIndex = this->_getCommandIndex(command);
    this->_commandCallbackPointers[commandIndex] = commandCallbackPointer;
}

void SimpleSerialProtocol::_unregisterCommandCallback(const byte command) const
{
    const uint8_t commandIndex = this->_getCommandIndex(command);
    this->_commandCallbackPointers[commandIndex] = nullptr;
}

bool SimpleSerialProtocol::_isCommandRangeValid() const
{
    const bool toIsLowerThanFrom = this->_commandCallbackRangeFrom > this->_commandCallbackRangeTo;
    const bool fromOrToIsLowerThanMinimum = this->_commandCallbackRangeFrom < COMMAND_CALLBACK_RANGE_FROM ||
        this->_commandCallbackRangeTo < COMMAND_CALLBACK_RANGE_FROM;
    const bool fromOrToIsGreaterThanMaximum = this->_commandCallbackRangeFrom > COMMAND_CALLBACK_RANGE_TO ||
        this->_commandCallbackRangeTo > COMMAND_CALLBACK_RANGE_TO;
    return !toIsLowerThanFrom && !fromOrToIsLowerThanMinimum && !fromOrToIsGreaterThanMaximum;
}

bool SimpleSerialProtocol::_isCommandInReservedRange(const byte command) const
{
    return command >= this->_commandCallbackRangeFrom && command <= this->_commandCallbackRangeTo;
}

bool SimpleSerialProtocol::_isCommandRegistered(const byte command) const
{
    const uint8_t commandIndex = this->_getCommandIndex(command);
    return this->_commandCallbackPointers[commandIndex] != nullptr;
}

uint8_t SimpleSerialProtocol::_getCommandIndex(const byte command) const
{
    return static_cast<uint8_t>(command) - this->_commandCallbackRangeFrom;
}

void SimpleSerialProtocol::_callCommandCallback(const byte command) const
{
    const uint8_t commandIndex = this->_getCommandIndex(command);
    const CallbackPointer commandCallbackPointer = this->_commandCallbackPointers[commandIndex];
    commandCallbackPointer();
}

void SimpleSerialProtocol::_error(const uint8_t errorNum, const bool dieImmediately)
{
    this->_errorCallbackPointer(errorNum);
    if (dieImmediately) this->_die();
}

/***************************** PRIVATE *********************************/
void SimpleSerialProtocol::_flushCommand()
{
    bool completlyFlushed = false;

    const unsigned long lastMillis = millis();

    while (!completlyFlushed)
    {
        if (this->streamPointer->available() > 0)
        {
            const byte bite = this->readByte();
            if (bite == CHAR_EOT)
            {
                completlyFlushed = true;
                this->_isWaitingForReadEot = false;
            }
        }

        if (millis() - lastMillis > FLUSH_TIMEOUT)
        {
            this->_die();
            return;
        }
    }
}

void SimpleSerialProtocol::_die()
{
    this->_isDead = true;
    this->_errorCallbackPointer(ERROR_IS_DEAD);
}
