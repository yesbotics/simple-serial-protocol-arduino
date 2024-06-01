#ifndef SSP_COMMON_H
#define SSP_COMMON_H

#include <Arduino.h>


#if defined(ARDUINO_AVR_UNO) \
    || defined(ARDUINO_AVR_NANO) \
    || defined(ARDUINO_ARCH_ESP32)
#define HARDWARESERIAL
#elif defined(ARDUINO_AVR_MICRO)
#define USBAPISERIAL
#endif

#if defined(ARDUINO_AVR_UNO) \
    || defined(ARDUINO_AVR_NANO) \
    || defined(ARDUINO_AVR_MICRO)
#define SOFTWARESERIAL_SUPPORTED
constexpr uint8_t STREAM_TYPE_SOFTWARESERIAL = 3;
#include <SoftwareSerial.h>
#endif


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


#endif //SSP_COMMON_H
