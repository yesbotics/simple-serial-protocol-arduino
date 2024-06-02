#include "SimpleSerialProtocol.h"

/************************************************************************/
/**************************** PUBLIC DTORS ******************************/
/************************************************************************/

SimpleSerialProtocol::~SimpleSerialProtocol()
{
    delete this->_cbErrPtr;
    delete[] this->_cbPtrs;
};

/************************************************************************/
/**************************** PUBLIC FCTS *******************************/
/************************************************************************/

void SimpleSerialProtocol::init()
{
    Core::init();
    if (!this->_isCommandRangeValid())
    {
        this->_error(ERROR_COMMAND_RANGE_IS_INVALID, true);
        return;
    }
    this->_isInitialized = true;
}

bool SimpleSerialProtocol::loop()
{
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

void SimpleSerialProtocol::registerCommand(const byte command, Callback* cbPtr)
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
    if (this->_isCommandRegistered(command))
    {
        this->_error(ERROR_COMMAND_IS_REGISTERED, true);
        return;
    }
    const uint8_t commandIndex = this->_getCommandIndex(command);
    this->_cbPtrs[commandIndex] = cbPtr;
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
    const uint8_t commandIndex = this->_getCommandIndex(command);
    this->_cbPtrs[commandIndex] = nullptr;
}

byte SimpleSerialProtocol::readCommand()
{
    const byte bite = this->readByte();
    this->_isWaitingForReadEot = true;
    return bite;
}

void SimpleSerialProtocol::writeCommand(const byte command) const
{
    this->writeByte(command);
}

byte SimpleSerialProtocol::readEot()
{
    // TODO: check, ob eot ausgelesen werden darf,
    // TODO: nur wenn this->_isWaitingForReadEot true ist

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

void SimpleSerialProtocol::writeEot() const
{
    this->writeByte(CHAR_EOT);
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

/************************************************************************/
/**************************** PROTECTED FCTS ****************************/
/************************************************************************/

void SimpleSerialProtocol::_onWaitForByteTimeout()
{
    this->_error(ERROR_WAIT_FOR_BYTE_TIMEOUT, true);
}

/************************************************************************/
/**************************** PRIVATE CTORS *****************************/
/************************************************************************/

SimpleSerialProtocol::SimpleSerialProtocol(
    const bool isSoftwareSerial,
    Stream* streamPtr,
    const uint32_t baudrate,
    const uint32_t waitForByteTimeout,
    const byte commandCallbackRangeFrom,
    const byte commandCallbackRangeTo,
    Callback* cbErrPtr
):
    Core{
        streamPtr,
        isSoftwareSerial,
        baudrate,
        waitForByteTimeout
    },
    _cbRangeFrom{commandCallbackRangeFrom},
    _cbRangeTo{commandCallbackRangeTo},
    _cbErrPtr{cbErrPtr}
{
    const uint8_t cbPtrsBufferMaxIndex = abs(_cbRangeTo - _cbRangeFrom);
    const uint8_t cbPtrsBufferSize = cbPtrsBufferMaxIndex + 1;
    this->_cbPtrs = new Callback*[cbPtrsBufferSize];
    for (uint8_t i = 0; i <= cbPtrsBufferMaxIndex; i++)
    {
        this->_cbPtrs[i] = nullptr;
    }
}

/************************************************************************/
/**************************** PRIVATE FCTS ******************************/
/************************************************************************/

bool SimpleSerialProtocol::_isCommandRangeValid() const
{
    const bool toIsLowerThanFrom = this->_cbRangeFrom > this->_cbRangeTo;
    const bool fromOrToIsLowerThanMinimum = this->_cbRangeFrom < COMMAND_CALLBACK_RANGE_FROM ||
        this->_cbRangeTo < COMMAND_CALLBACK_RANGE_FROM;
    const bool fromOrToIsGreaterThanMaximum = this->_cbRangeFrom > COMMAND_CALLBACK_RANGE_TO ||
        this->_cbRangeTo > COMMAND_CALLBACK_RANGE_TO;
    return !toIsLowerThanFrom && !fromOrToIsLowerThanMinimum && !fromOrToIsGreaterThanMaximum;
}

bool SimpleSerialProtocol::_isCommandInReservedRange(const byte command) const
{
    return command >= this->_cbRangeFrom && command <= this->_cbRangeTo;
}

bool SimpleSerialProtocol::_isCommandRegistered(const byte command) const
{
    const uint8_t commandIndex = this->_getCommandIndex(command);
    return this->_cbPtrs[commandIndex] != nullptr;
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
        this->_error(ERROR_COMMAND_IS_NOT_REGISTERED, true);
        this->_flushCommand();
        return;
    }
    this->_callCommandCallback(command);
}

uint8_t SimpleSerialProtocol::_getCommandIndex(const byte command) const
{
    return (static_cast<uint8_t>(command) - this->_cbRangeFrom);
}

void SimpleSerialProtocol::_callCommandCallback(const byte command) const
{
    const uint8_t commandIndex = this->_getCommandIndex(command);
    const Callback* cbPtr = this->_cbPtrs[commandIndex];
    if (cbPtr != nullptr) cbPtr->execute();
}

void SimpleSerialProtocol::_error(const uint8_t errorNum, const bool dieImmediately)
{
    this->_callErrorCallback(errorNum);
    if (dieImmediately) this->_die();
}

void SimpleSerialProtocol::_callErrorCallback(const uint8_t errorNum) const
{
    if (this->_cbErrPtr != nullptr) _cbErrPtr->execute(errorNum);
}

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
