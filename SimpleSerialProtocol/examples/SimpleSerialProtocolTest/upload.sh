curl localhost:3000/disconnect
arduino-cli compile --fqbn arduino:avr:nano:cpu=atmega328old
arduino-cli upload -p COM4 --fqbn arduino:avr:nano:cpu=atmega328old