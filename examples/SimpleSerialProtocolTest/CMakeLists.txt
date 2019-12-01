cmake_minimum_required(VERSION 2.8.4)

# Toolchain - Nix zu ändern
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-fpermissive -std=c++0x")

# Pfad zu den Extra-Libraries ermitteln - eventuell unter Linux anzupassen - Vielleicht findet man hier eine generelle Lösung
file(TO_CMAKE_PATH "$ENV{HOMEDRIVE}$ENV{HOMEPATH}/Documents/Arduino/libraries" ARDUINO_EXTRA_LIBRARIES_PATH)

# Projektname - wird aus Ordnername generiert
get_filename_component(PROJECT_NAME "${CMAKE_CURRENT_LIST_DIR}" NAME)

#
# Board
# Da das CMake nur für die Autovervollständigung genutzt wird, ist die CPU/Board eventuell nicht so wichtig
#
# OPTIONAL
#
set(ARDUINO_CPU atmega328old)
set(${PROJECT_NAME}_BOARD nano)

project(${PROJECT_NAME})

#
# Arduino-Konforme-Bibliotheken
#
link_directories(
)

#
# NON-Standard-Bibliotheken
# Für Bibliotheken, die sich nicht im Standardordner befinden oder Bibliotheken, die NICHT die Standardform (Lib-Name == Ordnername)
# einhalten, müssen diese hier mit included werden. Manchmal auch Hardware-Libs.
#
# OPTIONAL
#
include_directories(
#        ${ARDUINO_EXTRA_LIBRARIES_PATH}/USB_Host_Shield_20
#        ${ARDUINO_SDK_PATH}/hardware/avr/libraries/SoftwareSerial/src
)

set(${PROJECT_NAME}_SKETCH ${PROJECT_NAME}.ino)
set(${PROJECT_NAME}_PROGRAMMER avrispmkii)
set(${PROJECT_NAME}_PORT COM3)
set(${PROJECT_NAME}_RECURSE True)
#set(${PROJECT_NAME}_AFLAGS -v)
generate_arduino_firmware(${PROJECT_NAME})
