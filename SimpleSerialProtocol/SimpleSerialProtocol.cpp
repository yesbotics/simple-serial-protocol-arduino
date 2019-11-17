#include "SimpleSerialProtocol.h"

/***************************** PUBLIC *********************************/

#ifdef SOFTWARESERIAL_SUPPORTED

SimpleSerialProtocol::SimpleSerialProtocol(
        SoftwareSerial &softwareSerialRef,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        ExternalFatalErrorCallbackPointer externalFatalErrorCallbackPointer,
        unsigned int externalCommandCallbackRangeFrom,
        unsigned int externalCommandCallbackRangeTo) :
        SimpleSerialProtocol(
                (Stream &) softwareSerialRef,
                STREAM_TYPE_SOFTWARESERIAL,
                baudrate, waitForByteTimeout,
                externalFatalErrorCallbackPointer,
                externalCommandCallbackRangeFrom,
                externalCommandCallbackRangeTo
        ) {}

SimpleSerialProtocol::SimpleSerialProtocol(
        SoftwareSerial *softwareSerialPtr,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        ExternalFatalErrorCallbackPointer externalFatalErrorCallbackPointer,
        unsigned int externalCommandCallbackRangeFrom,
        unsigned int externalCommandCallbackRangeTo) :
        SimpleSerialProtocol(
                (Stream *) softwareSerialPtr,
                STREAM_TYPE_SOFTWARESERIAL,
                baudrate,
                waitForByteTimeout,
                externalFatalErrorCallbackPointer,
                externalCommandCallbackRangeFrom,
                externalCommandCallbackRangeTo
        ) {}

#endif

SimpleSerialProtocol::SimpleSerialProtocol(
        HardwareSerial &hardwareSerialRef,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        ExternalFatalErrorCallbackPointer externalFatalErrorCallbackPointer,
        unsigned int externalCommandCallbackRangeFrom,
        unsigned int externalCommandCallbackRangeTo) :
        SimpleSerialProtocol(
                (Stream &) hardwareSerialRef,
                STREAM_TYPE_HARDWARESERIAL,
                baudrate,
                waitForByteTimeout,
                externalFatalErrorCallbackPointer,
                externalCommandCallbackRangeFrom,
                externalCommandCallbackRangeTo
        ) {}

SimpleSerialProtocol::SimpleSerialProtocol(
        HardwareSerial *hardwareSerialPtr,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        ExternalFatalErrorCallbackPointer externalFatalErrorCallbackPointer,
        unsigned int externalCommandCallbackRangeFrom,
        unsigned int externalCommandCallbackRangeTo) :
        SimpleSerialProtocol(
                (Stream *) hardwareSerialPtr,
                STREAM_TYPE_HARDWARESERIAL,
                baudrate,
                waitForByteTimeout,
                externalFatalErrorCallbackPointer,
                externalCommandCallbackRangeFrom,
                externalCommandCallbackRangeTo
        ) {}


SimpleSerialProtocol::SimpleSerialProtocol(
        Stream &streamRef,
        unsigned int streamType,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        ExternalFatalErrorCallbackPointer externalFatalErrorCallbackPointer,
        unsigned int externalCommandCallbackRangeFrom ,
        unsigned int externalCommandCallbackRangeTo) :
        Core(
                streamRef,
                streamType,
                baudrate,
                waitForByteTimeout
        ) {

    this->afterConstructor(
            externalFatalErrorCallbackPointer,
            externalCommandCallbackRangeFrom,
            externalCommandCallbackRangeTo
    );
}


SimpleSerialProtocol::SimpleSerialProtocol(
        Stream *streamPtr,
        unsigned int streamType,
        unsigned long baudrate,
        unsigned long waitForByteTimeout,
        ExternalFatalErrorCallbackPointer externalFatalErrorCallbackPointer,
        unsigned int externalCommandCallbackRangeFrom,
        unsigned int externalCommandCallbackRangeTo) :
        Core(
                streamPtr,
                streamType,
                baudrate,
                waitForByteTimeout
        ) {
    this->afterConstructor(
            externalFatalErrorCallbackPointer,
            externalCommandCallbackRangeFrom,
            externalCommandCallbackRangeTo
    );
}

SimpleSerialProtocol::~SimpleSerialProtocol() {

}

void SimpleSerialProtocol::afterConstructor(
        ExternalFatalErrorCallbackPointer externalFatalErrorCallbackPointer,
        unsigned int externalCommandCallbackRangeFrom,
        unsigned int externalCommandCallbackRangeTo
) {
    this->externalFatalErrorCallbackPointer = externalFatalErrorCallbackPointer;

    this->externalCommandCallbackRangeFrom = externalCommandCallbackRangeFrom;
    this->externalCommandCallbackRangeTo = externalCommandCallbackRangeTo;

    unsigned int externalCommandCallbackPointerBufferSize =
            abs(externalCommandCallbackRangeTo - externalCommandCallbackRangeFrom) + 1;

    this->externalCommandCallbackPointers = new ExternalCallbackPointer[externalCommandCallbackPointerBufferSize];
    for (unsigned int i = 0; i < externalCommandCallbackPointerBufferSize;i++) {
        this->externalCommandCallbackPointers[i] = 0;
    }
}

void SimpleSerialProtocol::init() {
    Core::init();
//	Serial.println(F("SimpleSerialProtocol::init"));
    if (!this->isExternalCommandRangeValid()) {
        this->fatalError(ERROR_EXTERNAL_COMMAND_RANGE_IS_INVALID, true);
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
        this->fatalError(ERROR_EOT_WAS_NOT_READ, true);
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
        this->fatalError(ERROR_IS_NOT_WAITING_FOR_READ_EOT, true);
        return CHAR_EOT;
    }

    this->isWaitingForReadEot = false;
    byte bite = this->readByte();
    if (bite != CHAR_EOT) {
        this->fatalError(ERROR_IS_NOT_EOT, true);
    }
    return bite;
}

void SimpleSerialProtocol::writeCommand(const byte command) {
    this->writeByte(command);
}

void SimpleSerialProtocol::writeEot() {
    this->writeByte(CHAR_EOT);
}

void SimpleSerialProtocol::registerCommand(const byte command, ExternalCallbackPointer externalCallbackPointer) {
//	Serial.println(F("SimpleSerialProtocol::registerCommand"));

    if (!this->_isInitialized) {
        this->fatalError(ERROR_IS_NOT_INITIALIZED, true);
        return;
    }

    if (!this->isExternalCommandInReservedRange(command)) {
        this->fatalError(ERROR_EXTERNAL_COMMAND_IS_NOT_IN_RESERVED_RANGE, true);
        return;
    }

    if (this->isExternalCommandRegistered(command)) {
        this->fatalError(ERROR_EXTERNAL_COMMAND_IS_REGISTERED, true);
        return;
    }

    this->registerExternalCommandCallback(command, externalCallbackPointer);
}

/***************************** PROTECTED *********************************/

void SimpleSerialProtocol::onWaitForByteTimeout() {
    this->fatalError(ERROR_WAIT_FOR_BYTE_TIMEOUT, true);
}

void SimpleSerialProtocol::onGotCommandByte(byte command) {
//	Serial.println(F("SimpleSer/ialProtocol::onGotCommandByte "));
//	Serial.println(command);

    if (!this->_isInitialized) {
        this->fatalError(ERROR_IS_NOT_INITIALIZED, true);
        return;
    }

    if (!this->isExternalCommandInReservedRange(command)) {
        this->fatalError(ERROR_EXTERNAL_COMMAND_IS_NOT_IN_RESERVED_RANGE, true);
        this->flushCommand();
        return;
    }

    if (!this->isExternalCommandRegistered(command)) {
        this->fatalError(ERROR_EXTERNAL_COMMAND_IS_NOT_REGISTERED, true);
        this->flushCommand();
        return;
    }

    this->callExternalCommandCallback(command);

}

void SimpleSerialProtocol::registerExternalCommandCallback(const byte command,
                                                           ExternalCallbackPointer externalCallbackPointer) {
    unsigned int externalCommandIndex = this->getExternalCommandIndex(command);
    this->externalCommandCallbackPointers[externalCommandIndex] = externalCallbackPointer;
}

bool SimpleSerialProtocol::isExternalCommandRangeValid() {
    bool toIsLowerThanFrom = this->externalCommandCallbackRangeFrom > this->externalCommandCallbackRangeTo;
    bool fromOrToIsLowerThanMinimum = this->externalCommandCallbackRangeFrom < EXTERNAL_COMMAND_CALLBACK_RANGE_FROM ||
                                      this->externalCommandCallbackRangeTo < EXTERNAL_COMMAND_CALLBACK_RANGE_FROM;
    bool fromOrToIsGreaterThanMaximum = this->externalCommandCallbackRangeFrom > EXTERNAL_COMMAND_CALLBACK_RANGE_TO ||
                                        this->externalCommandCallbackRangeTo > EXTERNAL_COMMAND_CALLBACK_RANGE_TO;
    return !toIsLowerThanFrom && !fromOrToIsLowerThanMinimum && !fromOrToIsGreaterThanMaximum;
}

bool SimpleSerialProtocol::isExternalCommandInReservedRange(byte command) {
    return command >= this->externalCommandCallbackRangeFrom && command <= this->externalCommandCallbackRangeTo;
}

bool SimpleSerialProtocol::isExternalCommandRegistered(byte command) {
    unsigned int externalCommandIndex = this->getExternalCommandIndex(command);
    return this->externalCommandCallbackPointers[externalCommandIndex] != 0;
}

unsigned int SimpleSerialProtocol::getExternalCommandIndex(byte command) {
    return (unsigned int) command - this->externalCommandCallbackRangeFrom;
}

void SimpleSerialProtocol::callExternalCommandCallback(byte command) {
    unsigned int externalCommandIndex = this->getExternalCommandIndex(command);
    ExternalCallbackPointer externalCommandCallbackPointer = this->externalCommandCallbackPointers[externalCommandIndex];
    externalCommandCallbackPointer();
}

void SimpleSerialProtocol::fatalError(unsigned int errorNum, bool dieInstantly) {
    this->externalFatalErrorCallbackPointer(errorNum);
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
    this->externalFatalErrorCallbackPointer(ERROR_IS_DEAD);
}
