#include "SimpleSerialProtocol.h"

/***************************** PUBLIC *********************************/

#ifdef SOFTWARESERIAL_SUPPORTED

SimpleSerialProtocol::SimpleSerialProtocol(
        SoftwareSerial& softwareSerialRef,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        ErrorCallbackPointer errorCallbackPointer,
        byte commandCallbackRangeFrom,
        byte commandCallbackRangeTo) :
        SimpleSerialProtocol(
                (Stream&) softwareSerialRef,
                STREAM_TYPE_SOFTWARESERIAL,
                baudrate, waitForByteTimeout,
                errorCallbackPointer,
                commandCallbackRangeFrom,
                commandCallbackRangeTo
        ) {}

SimpleSerialProtocol::SimpleSerialProtocol(
        SoftwareSerial* softwareSerialPtr,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        ErrorCallbackPointer errorCallbackPointer,
        byte commandCallbackRangeFrom,
        byte commandCallbackRangeTo) :
        SimpleSerialProtocol(
                (Stream*) softwareSerialPtr,
                STREAM_TYPE_SOFTWARESERIAL,
                baudrate,
                waitForByteTimeout,
                errorCallbackPointer,
                commandCallbackRangeFrom,
                commandCallbackRangeTo
        ) {}

#endif

SimpleSerialProtocol::SimpleSerialProtocol(
        HardwareSerial& hardwareSerialRef,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        ErrorCallbackPointer errorCallbackPointer,
        byte commandCallbackRangeFrom,
        byte commandCallbackRangeTo) :
        SimpleSerialProtocol(
                (Stream&) hardwareSerialRef,
                STREAM_TYPE_HARDWARESERIAL,
                baudrate,
                waitForByteTimeout,
                errorCallbackPointer,
                commandCallbackRangeFrom,
                commandCallbackRangeTo
        ) {}

SimpleSerialProtocol::SimpleSerialProtocol(
        HardwareSerial* hardwareSerialPtr,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        ErrorCallbackPointer errorCallbackPointer,
        byte commandCallbackRangeFrom,
        byte commandCallbackRangeTo) :
        SimpleSerialProtocol(
                (Stream*) hardwareSerialPtr,
                STREAM_TYPE_HARDWARESERIAL,
                baudrate,
                waitForByteTimeout,
                errorCallbackPointer,
                commandCallbackRangeFrom,
                commandCallbackRangeTo
        ) {}


SimpleSerialProtocol::SimpleSerialProtocol(
        Stream& streamRef,
        uint8_t streamType,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        ErrorCallbackPointer errorCallbackPointer,
        byte commandCallbackRangeFrom,
        byte commandCallbackRangeTo) :
        Core(
                streamRef,
                streamType,
                baudrate,
                waitForByteTimeout
        ) {

    this->afterConstructor(
            errorCallbackPointer,
            commandCallbackRangeFrom,
            commandCallbackRangeTo
    );
}


SimpleSerialProtocol::SimpleSerialProtocol(
        Stream* streamPtr,
        uint8_t streamType,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        ErrorCallbackPointer errorCallbackPointer,
        byte commandCallbackRangeFrom,
        byte commandCallbackRangeTo) :
        Core(
                streamPtr,
                streamType,
                baudrate,
                waitForByteTimeout
        ) {
    this->afterConstructor(
            errorCallbackPointer,
            commandCallbackRangeFrom,
            commandCallbackRangeTo
    );
}

SimpleSerialProtocol::~SimpleSerialProtocol() {

}

void SimpleSerialProtocol::afterConstructor(
        ErrorCallbackPointer errorCallbackPointer,
        byte commandCallbackRangeFrom,
        byte commandCallbackRangeTo
) {
    this->errorCallbackPointer = errorCallbackPointer;

    this->commandCallbackRangeFrom = commandCallbackRangeFrom;
    this->commandCallbackRangeTo = commandCallbackRangeTo;

    byte commandCallbackPointerBufferMaxIndex = abs(commandCallbackRangeTo - commandCallbackRangeFrom);
    uint16_t commandCallbackPointerBufferSize = uint16_t(commandCallbackPointerBufferMaxIndex) + 1;

    this->commandCallbackPointers = new CallbackPointer[commandCallbackPointerBufferSize];

    for (uint16_t i = 0; i <= commandCallbackPointerBufferMaxIndex; i++) {
        this->commandCallbackPointers[i] = 0;
    }
}

void SimpleSerialProtocol::init() {
    Core::init();
//    Serial.println(F("SimpleSerialProtocol::init"));
    if (!this->isCommandRangeValid()) {
        this->error(ERROR_COMMAND_RANGE_IS_INVALID, true);
        return;
    }
    this->_isInitialized = true;
}

bool SimpleSerialProtocol::loop() {
//	Serial.print(F("SimpleSerialProtocol::loop"));

    if (this->_isDead) {
        return false;
    }

    if (this->isWaitingForReadEot) {
        this->error(ERROR_EOT_WAS_NOT_READ, true);
        return false;
    }

    if (this->streamPointer->available() > 0) {
        byte command = this->readCommand();
        this->onGotCommandByte(command);
    }

    return true;
}

byte SimpleSerialProtocol::readCommand() {
    byte bite = this->readByte();
    this->isWaitingForReadEot = true;
    return bite;
}

byte SimpleSerialProtocol::readEot() {
//TODO: check, ob eot ausgelesen werden darf, nur wenn this->isWaitingForReadEot true ist

    if (!this->isWaitingForReadEot) {
        this->error(ERROR_IS_NOT_WAITING_FOR_READ_EOT, true);
        return CHAR_EOT;
    }

    this->isWaitingForReadEot = false;
    byte bite = this->readByte();
    if (bite != CHAR_EOT) {
        this->error(ERROR_IS_NOT_EOT, true);
    }
    return bite;
}

void SimpleSerialProtocol::writeCommand(const byte command) {
    this->writeByte(command);
}

void SimpleSerialProtocol::writeEot() {
    this->writeByte(CHAR_EOT);
}

void SimpleSerialProtocol::setDieInstantlyOnNotRegisteredCommand(bool die) {
    this->dieInstantlyOnNotRegisteredCommand = die;
}

void SimpleSerialProtocol::registerCommand(const byte command, CallbackPointer callbackPointer) {
//	Serial.println(F("SimpleSerialProtocol::registerCommand"));

    if (!this->_isInitialized) {
        this->error(ERROR_IS_NOT_INITIALIZED, true);
        return;
    }

    if (!this->isCommandInReservedRange(command)) {
        this->error(ERROR_COMMAND_IS_NOT_IN_RESERVED_RANGE, true);
        return;
    }

    if (this->isCommandRegistered(command)) {
        this->error(ERROR_COMMAND_IS_REGISTERED, true);
        return;
    }

    this->registerCommandCallback(command, callbackPointer);
}

bool SimpleSerialProtocol::readCString(char* output, uint8_t maxLength) {
    bool successful = Core::readCString(output, maxLength);
    if (!successful) {
        this->error(ERROR_END_OF_STRING_BYTE_NOT_IN_CHAR_ARRAY, true);
    }
    return successful;
}

/***************************** PROTECTED *********************************/

void SimpleSerialProtocol::onWaitForByteTimeout() {
    this->error(ERROR_WAIT_FOR_BYTE_TIMEOUT, true);
}

void SimpleSerialProtocol::onGotCommandByte(byte command) {
//	Serial.println(F("SimpleSer/ialProtocol::onGotCommandByte "));
//	Serial.println(command);

    if (!this->_isInitialized) {
        this->error(ERROR_IS_NOT_INITIALIZED, true);
        return;
    }

    if (!this->isCommandInReservedRange(command)) {
        this->error(ERROR_COMMAND_IS_NOT_IN_RESERVED_RANGE, true);
        this->flushCommand();
        return;
    }

    if (!this->isCommandRegistered(command)) {
        this->error(ERROR_COMMAND_IS_NOT_REGISTERED, dieInstantlyOnNotRegisteredCommand);
        this->flushCommand();
        return;
    }

    this->callCommandCallback(command);

}

void SimpleSerialProtocol::registerCommandCallback(const byte command, CallbackPointer callbackPointer) {
    uint8_t commandIndex = this->getCommandIndex(command);
    this->commandCallbackPointers[commandIndex] = callbackPointer;
}

bool SimpleSerialProtocol::isCommandRangeValid() {
    bool toIsLowerThanFrom = this->commandCallbackRangeFrom > this->commandCallbackRangeTo;
    bool fromOrToIsLowerThanMinimum = this->commandCallbackRangeFrom < COMMAND_CALLBACK_RANGE_FROM ||
                                      this->commandCallbackRangeTo < COMMAND_CALLBACK_RANGE_FROM;
    bool fromOrToIsGreaterThanMaximum = this->commandCallbackRangeFrom > COMMAND_CALLBACK_RANGE_TO ||
                                        this->commandCallbackRangeTo > COMMAND_CALLBACK_RANGE_TO;
    return !toIsLowerThanFrom && !fromOrToIsLowerThanMinimum && !fromOrToIsGreaterThanMaximum;
}

bool SimpleSerialProtocol::isCommandInReservedRange(byte command) {
    return command >= this->commandCallbackRangeFrom && command <= this->commandCallbackRangeTo;
}

bool SimpleSerialProtocol::isCommandRegistered(byte command) {
    uint8_t commandIndex = this->getCommandIndex(command);
    return this->commandCallbackPointers[commandIndex] != 0;
}

uint8_t SimpleSerialProtocol::getCommandIndex(byte command) {
    return (uint8_t) command - this->commandCallbackRangeFrom;
}

void SimpleSerialProtocol::callCommandCallback(byte command) {
    uint8_t commandIndex = this->getCommandIndex(command);
    CallbackPointer commandCallbackPointer = this->commandCallbackPointers[commandIndex];
    commandCallbackPointer();
}

void SimpleSerialProtocol::error(uint8_t errorNum, bool dieInstantly) {
    this->errorCallbackPointer(errorNum);
    if (dieInstantly) {
        this->die();
        return;
    }
}

/***************************** PRIVATE *********************************/
void SimpleSerialProtocol::flushCommand() {
//	Serial.println(F("SimpleSerialProtocol::flushCommand"));

    bool completlyFlushed = false;

    unsigned long lastMillis = millis();

    while (!completlyFlushed) {

        if (this->streamPointer->available() > 0) {
            byte bite = this->readByte();
            if (bite == CHAR_EOT) {
                completlyFlushed = true;
                this->isWaitingForReadEot = false;
            }
        }

        if (millis() - lastMillis > FLUSH_TIMEOUT) {
            this->die();
            return;
        }

    }

}

void SimpleSerialProtocol::die() {
//	Serial.println(F("SimpleSerialProtocol::die"));
    this->_isDead = true;
    this->errorCallbackPointer(ERROR_IS_DEAD);
}
