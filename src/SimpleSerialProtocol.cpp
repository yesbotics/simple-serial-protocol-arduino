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
    const ErrorCallbackPointer errorCallbackPointer,
    const byte commandCallbackRangeFrom,
    const byte commandCallbackRangeTo) :
    SimpleSerialProtocol{
        static_cast<Stream&>(usbapiSerialRef),
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
    Serial_* usbapiSerialPtr,
    const unsigned long baudrate,
    const unsigned long waitForByteTimeout,
    const ErrorCallbackPointer errorCallbackPointer,
    const byte commandCallbackRangeFrom,
    const byte commandCallbackRangeTo) :
    SimpleSerialProtocol{
        static_cast<Stream*>(usbapiSerialPtr),
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
    commandCallbackRangeFrom{commandCallbackRangeFrom},
    commandCallbackRangeTo{commandCallbackRangeTo},
    errorCallbackPointer{errorCallbackPointer}
{
    afterConstructor();
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
    commandCallbackRangeFrom{commandCallbackRangeFrom},
    commandCallbackRangeTo{commandCallbackRangeTo},
    errorCallbackPointer{errorCallbackPointer}
{
    afterConstructor();
}

SimpleSerialProtocol::~SimpleSerialProtocol()
{
    delete[] this->commandCallbackPointers;
}

void SimpleSerialProtocol::afterConstructor()
{
    const byte commandCallbackPointerBufferMaxIndex = abs(commandCallbackRangeTo - commandCallbackRangeFrom);
    const uint16_t commandCallbackPointerBufferSize = static_cast<uint16_t>(commandCallbackPointerBufferMaxIndex) + 1;
    this->commandCallbackPointers = new CallbackPointer[commandCallbackPointerBufferSize];
    for (uint16_t i = 0; i <= commandCallbackPointerBufferMaxIndex; i++)
    {
        this->commandCallbackPointers[i] = nullptr;
    }
}

void SimpleSerialProtocol::init()
{
    Core::init();
    //    Serial.println(F("SimpleSerialProtocol::init"));
    if (!this->isCommandRangeValid())
    {
        this->error(ERROR_COMMAND_RANGE_IS_INVALID, true);
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

    if (this->isWaitingForReadEot)
    {
        this->error(ERROR_EOT_WAS_NOT_READ, true);
        return false;
    }

    if (this->streamPointer->available() > 0)
    {
        const byte command = this->readCommand();
        this->onGotCommandByte(command);
    }

    return true;
}

byte SimpleSerialProtocol::readCommand()
{
    const byte bite = this->readByte();
    this->isWaitingForReadEot = true;
    return bite;
}

byte SimpleSerialProtocol::readEot()
{
    //TODO: check, ob eot ausgelesen werden darf, nur wenn this->isWaitingForReadEot true ist

    if (!this->isWaitingForReadEot)
    {
        this->error(ERROR_IS_NOT_WAITING_FOR_READ_EOT, true);
        return CHAR_EOT;
    }

    this->isWaitingForReadEot = false;
    const byte bite = this->readByte();
    if (bite != CHAR_EOT)
    {
        this->error(ERROR_IS_NOT_EOT, true);
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

void SimpleSerialProtocol::setDieInstantlyOnNotRegisteredCommand(const bool die)
{
    this->dieImmediatelyOnNotRegisteredCommand = die;
}

void SimpleSerialProtocol::registerCommand(const byte command, const CallbackPointer callbackPointer)
{
    //	Serial.println(F("SimpleSerialProtocol::registerCommand"));

    if (!this->_isInitialized)
    {
        this->error(ERROR_IS_NOT_INITIALIZED, true);
        return;
    }

    if (!this->isCommandInReservedRange(command))
    {
        this->error(ERROR_COMMAND_IS_NOT_IN_RESERVED_RANGE, true);
        return;
    }

    if (this->isCommandRegistered(command))
    {
        this->error(ERROR_COMMAND_IS_REGISTERED, true);
        return;
    }

    this->registerCommandCallback(command, callbackPointer);
}

void SimpleSerialProtocol::unregisterCommand(const byte command)
{
    if (!this->_isInitialized)
    {
        this->error(ERROR_IS_NOT_INITIALIZED, true);
        return;
    }

    if (!this->isCommandInReservedRange(command))
    {
        this->error(ERROR_COMMAND_IS_NOT_IN_RESERVED_RANGE, true);
        return;
    }

    if (!this->isCommandRegistered(command))
    {
        this->error(ERROR_COMMAND_IS_NOT_REGISTERED, true);
        return;
    }

    this->unregisterCommandCallback(command);
}

bool SimpleSerialProtocol::readCString(char* output, const uint8_t maxLength)
{
    const bool successful = Core::readCString(output, maxLength);
    if (!successful)
    {
        this->error(ERROR_END_OF_STRING_BYTE_NOT_IN_CHAR_ARRAY, true);
    }
    return successful;
}

/***************************** PROTECTED *********************************/

void SimpleSerialProtocol::onWaitForByteTimeout()
{
    this->error(ERROR_WAIT_FOR_BYTE_TIMEOUT, true);
}

void SimpleSerialProtocol::onGotCommandByte(const byte command)
{
    if (!this->_isInitialized)
    {
        this->error(ERROR_IS_NOT_INITIALIZED, true);
        return;
    }

    if (!this->isCommandInReservedRange(command))
    {
        this->error(ERROR_COMMAND_IS_NOT_IN_RESERVED_RANGE, true);
        this->flushCommand();
        return;
    }

    if (!this->isCommandRegistered(command))
    {
        this->error(ERROR_COMMAND_IS_NOT_REGISTERED, dieImmediatelyOnNotRegisteredCommand);
        this->flushCommand();
        return;
    }

    this->callCommandCallback(command);
}

void SimpleSerialProtocol::registerCommandCallback(
    const byte command,
    const CallbackPointer commandCallbackPointer
) const
{
    const uint8_t commandIndex = this->getCommandIndex(command);
    this->commandCallbackPointers[commandIndex] = commandCallbackPointer;
}

void SimpleSerialProtocol::unregisterCommandCallback(const byte command) const
{
    const uint8_t commandIndex = this->getCommandIndex(command);
    this->commandCallbackPointers[commandIndex] = nullptr;
}

bool SimpleSerialProtocol::isCommandRangeValid() const
{
    const bool toIsLowerThanFrom = this->commandCallbackRangeFrom > this->commandCallbackRangeTo;
    const bool fromOrToIsLowerThanMinimum = this->commandCallbackRangeFrom < COMMAND_CALLBACK_RANGE_FROM ||
        this->commandCallbackRangeTo < COMMAND_CALLBACK_RANGE_FROM;
    const bool fromOrToIsGreaterThanMaximum = this->commandCallbackRangeFrom > COMMAND_CALLBACK_RANGE_TO ||
        this->commandCallbackRangeTo > COMMAND_CALLBACK_RANGE_TO;
    return !toIsLowerThanFrom && !fromOrToIsLowerThanMinimum && !fromOrToIsGreaterThanMaximum;
}

bool SimpleSerialProtocol::isCommandInReservedRange(const byte command) const
{
    return command >= this->commandCallbackRangeFrom && command <= this->commandCallbackRangeTo;
}

bool SimpleSerialProtocol::isCommandRegistered(const byte command) const
{
    const uint8_t commandIndex = this->getCommandIndex(command);
    return this->commandCallbackPointers[commandIndex] != nullptr;
}

uint8_t SimpleSerialProtocol::getCommandIndex(const byte command) const
{
    return static_cast<uint8_t>(command) - this->commandCallbackRangeFrom;
}

void SimpleSerialProtocol::callCommandCallback(const byte command) const
{
    const uint8_t commandIndex = this->getCommandIndex(command);
    const CallbackPointer commandCallbackPointer = this->commandCallbackPointers[commandIndex];
    commandCallbackPointer();
}

void SimpleSerialProtocol::error(const uint8_t errorNum, const bool dieImmediately)
{
    this->errorCallbackPointer(errorNum);
    if (dieImmediately) this->die();
}

/***************************** PRIVATE *********************************/
void SimpleSerialProtocol::flushCommand()
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
                this->isWaitingForReadEot = false;
            }
        }

        if (millis() - lastMillis > FLUSH_TIMEOUT)
        {
            this->die();
            return;
        }
    }
}

void SimpleSerialProtocol::die()
{
    this->_isDead = true;
    this->errorCallbackPointer(ERROR_IS_DEAD);
}
