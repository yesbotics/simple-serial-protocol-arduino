#include "SimpleSerialProtocol.h"

/***************************** PUBLIC *********************************/

#ifdef SOFTWARESERIAL_SUPPORTED

SimpleSerialProtocol::SimpleSerialProtocol(
    SoftwareSerial& softwareSerialRef,
    const unsigned long baudrate,
    const unsigned long waitForByteTimeout,
    const StandaloneErrorCallbackPointer standaloneErrorCallbackPointer,
    const byte commandCallbackRangeFrom,
    const byte commandCallbackRangeTo) :
    SimpleSerialProtocol{
        static_cast<Stream&>(softwareSerialRef),
        true,
        baudrate,
        waitForByteTimeout,
        standaloneErrorCallbackPointer,
        commandCallbackRangeFrom,
        commandCallbackRangeTo
    }
{
}

SimpleSerialProtocol::SimpleSerialProtocol(
    SoftwareSerial* softwareSerialPtr,
    const unsigned long baudrate,
    const unsigned long waitForByteTimeout,
    const StandaloneErrorCallbackPointer standaloneErrorCallbackPointer,
    const byte commandCallbackRangeFrom,
    const byte commandCallbackRangeTo) :
    SimpleSerialProtocol{
        static_cast<Stream*>(softwareSerialPtr),
        true,
        baudrate,
        waitForByteTimeout,
        standaloneErrorCallbackPointer,
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
    const StandaloneErrorCallbackPointer standaloneErrorCallbackPointer,
    const byte commandCallbackRangeFrom,
    const byte commandCallbackRangeTo) :
    SimpleSerialProtocol{
        static_cast<Stream&>(hardwareSerialRef),
        false,
        baudrate,
        waitForByteTimeout,
        standaloneErrorCallbackPointer,
        commandCallbackRangeFrom,
        commandCallbackRangeTo
    }
{
}

SimpleSerialProtocol::SimpleSerialProtocol(
    HardwareSerial* hardwareSerialPtr,
    const unsigned long baudrate,
    const unsigned long waitForByteTimeout,
    const StandaloneErrorCallbackPointer standaloneErrorCallbackPointer,
    const byte commandCallbackRangeFrom,
    const byte commandCallbackRangeTo) :
    SimpleSerialProtocol{
        static_cast<Stream*>(hardwareSerialPtr),
        false,
        baudrate,
        waitForByteTimeout,
        standaloneErrorCallbackPointer,
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
    const StandaloneErrorCallbackPointer standaloneErrorCallbackPointer,
    const byte commandCallbackRangeFrom,
    const byte commandCallbackRangeTo) :
    SimpleSerialProtocol{
        static_cast<Stream&>(usbapiSerialRef),
        false,
        baudrate,
        waitForByteTimeout,
        standaloneErrorCallbackPointer,
        commandCallbackRangeFrom,
        commandCallbackRangeTo
    }
{
}

SimpleSerialProtocol::SimpleSerialProtocol(
    Serial_* usbapiSerialPtr,
    const unsigned long baudrate,
    const unsigned long waitForByteTimeout,
    const StandaloneErrorCallbackPointer standaloneErrorCallbackPointer,
    const byte commandCallbackRangeFrom,
    const byte commandCallbackRangeTo) :
    SimpleSerialProtocol{
        static_cast<Stream*>(usbapiSerialPtr),
        false,
        baudrate,
        waitForByteTimeout,
        standaloneErrorCallbackPointer,
        commandCallbackRangeFrom,
        commandCallbackRangeTo
    }
{
}

#endif


SimpleSerialProtocol::SimpleSerialProtocol(
    Stream& streamRef,
    const bool isSoftwareSerial,
    const unsigned long baudrate,
    const unsigned long waitForByteTimeout,
    const StandaloneErrorCallbackPointer standaloneErrorCallbackPointer,
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
    _standaloneErrorCallbackPointer{standaloneErrorCallbackPointer}
{
    _afterConstructor();
}


SimpleSerialProtocol::SimpleSerialProtocol(
    Stream* streamPtr,
    const bool isSoftwareSerial,
    const unsigned long baudrate,
    const unsigned long waitForByteTimeout,
    const StandaloneErrorCallbackPointer standaloneErrorCallbackPointer,
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
    _standaloneErrorCallbackPointer{standaloneErrorCallbackPointer}
{
    _afterConstructor();
}

SimpleSerialProtocol::~SimpleSerialProtocol()
{
    delete[] this->_standaloneCommandCallbackPointers;
    delete[] this->_instancesOfMemberCallbackPointers;
    delete[] this->_memberCommandCallbackPointers;
}

void SimpleSerialProtocol::_afterConstructor()
{
    const byte commandCallbackPointerBufferMaxIndex = abs(_commandCallbackRangeTo - _commandCallbackRangeFrom);
    const uint16_t commandCallbackPointerBufferSize = static_cast<uint16_t>(commandCallbackPointerBufferMaxIndex) + 1;
    this->_standaloneCommandCallbackPointers = new StandaloneCallbackPointer[commandCallbackPointerBufferSize];
    for (uint16_t i = 0; i <= commandCallbackPointerBufferMaxIndex; i++)
    {
        this->_standaloneCommandCallbackPointers[i] = nullptr;
    }
    this->_instancesOfMemberCallbackPointers = new VoidPointer[commandCallbackPointerBufferSize];
    for (uint16_t i = 0; i <= commandCallbackPointerBufferMaxIndex; i++)
    {
        this->_instancesOfMemberCallbackPointers[i] = nullptr;
    }
    this->_memberCommandCallbackPointers = new SspMemberCallbackPointer[commandCallbackPointerBufferSize];
    for (uint16_t i = 0; i <= commandCallbackPointerBufferMaxIndex; i++)
    {
        this->_memberCommandCallbackPointers[i] = nullptr;
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

void SimpleSerialProtocol::registerCommand(
    const byte command,
    const StandaloneCallbackPointer standaloneCommandCallbackPointer
)
{
    if (!this->_registerCommandPrecheck(command)) return;
    const uint8_t commandIndex = this->_getCommandIndex(command);
    this->_standaloneCommandCallbackPointers[commandIndex] = standaloneCommandCallbackPointer;
}

void SimpleSerialProtocol::unregisterCommand(const byte command)
{
    if (!this->_unregisterCommandPrecheck(command)) return;
    const uint8_t commandIndex = this->_getCommandIndex(command);
    this->_standaloneCommandCallbackPointers[commandIndex] = nullptr;
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

bool SimpleSerialProtocol::_registerCommandPrecheck(const byte command)
{
    if (!this->_isInitialized)
    {
        this->_error(ERROR_IS_NOT_INITIALIZED, true);
        return false;
    }

    if (!this->_isCommandInReservedRange(command))
    {
        this->_error(ERROR_COMMAND_IS_NOT_IN_RESERVED_RANGE, true);
        return false;
    }

    if (this->_isCommandRegistered(command))
    {
        this->_error(ERROR_COMMAND_IS_REGISTERED, true);
        return false;
    }

    return true;
}

bool SimpleSerialProtocol::_unregisterCommandPrecheck(const byte command)
{
    if (!this->_isInitialized)
    {
        this->_error(ERROR_IS_NOT_INITIALIZED, true);
        return false;
    }

    if (!this->_isCommandInReservedRange(command))
    {
        this->_error(ERROR_COMMAND_IS_NOT_IN_RESERVED_RANGE, true);
        return false;
    }

    if (!this->_isCommandRegistered(command))
    {
        this->_error(ERROR_COMMAND_IS_NOT_REGISTERED, true);
        return false;
    }
    return true;
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
    return this->_standaloneCommandCallbackPointers[commandIndex] != nullptr || (
        this->_instancesOfMemberCallbackPointers[commandIndex] != nullptr
        && this->_memberCommandCallbackPointers[commandIndex] != nullptr
    );
}

uint8_t SimpleSerialProtocol::_getCommandIndex(const byte command) const
{
    return static_cast<uint8_t>(command) - this->_commandCallbackRangeFrom;
}

void SimpleSerialProtocol::_callCommandCallback(const byte command) const
{

    Serial.println("XXX1");
    const uint8_t commandIndex = this->_getCommandIndex(command);

    if (this->_standaloneCommandCallbackPointers[commandIndex] != nullptr)
    {
        const StandaloneCallbackPointer commandCallbackPointer = this->_standaloneCommandCallbackPointers[commandIndex];
        commandCallbackPointer();
    }
    Serial.println("XXX2");
    if (
        this->_instancesOfMemberCallbackPointers[commandIndex] != nullptr
        && this->_memberCommandCallbackPointers[commandIndex] != nullptr
    )
    {
        Serial.println("XXX3");
        const auto object = static_cast<SspPointer>(this->_instancesOfMemberCallbackPointers[commandIndex]);
        const auto memberFunction = reinterpret_cast<SspMemberCallbackPointer>(this->_memberCommandCallbackPointers[commandIndex]);
        (object->*memberFunction)();
        Serial.println("XXX4");
    }
}

void SimpleSerialProtocol::_error(const uint8_t errorNum, const bool dieImmediately)
{
    this->_callErrorCallback(errorNum);
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
    this->_callErrorCallback(ERROR_IS_DEAD);
}

void SimpleSerialProtocol::_callErrorCallback(const uint8_t errorNum) const
{
    if (this->_standaloneErrorCallbackPointer != nullptr)
    {
        this->_standaloneErrorCallbackPointer(errorNum);
    }
    if (this->_instanceOfMemberErrorCallbackPointer != nullptr && this->_memberErrorCallbackPointer != nullptr)
    {
        // Cast the object back to its original type and call the member function
        const auto object = static_cast<SspPointer>(_instanceOfMemberErrorCallbackPointer);
        const auto memberFunction = reinterpret_cast<SspMemberErrorCallbackPointer>(_memberErrorCallbackPointer);
        (object->*memberFunction)(errorNum);
    }
}
