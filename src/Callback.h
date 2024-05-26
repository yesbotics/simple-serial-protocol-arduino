#ifndef CALLBACK_H
#define CALLBACK_H

#include <Arduino.h>

class Callback {
public:
    void execute();
private:
};

constexpr uint8_t CALLBACK_POINTER_SIZE = sizeof(Callback*);

#endif //CALLBACK_H
