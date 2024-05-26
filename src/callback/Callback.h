#ifndef CALLBACK_H
#define CALLBACK_H

#include <Arduino.h>

#include "StandaloneCallback.h"
#include "MemberCallback.h"
#include "ConstMemberCallback.h"

// Wrapper class for callbacks
class Callback
{
public:
    // Constructor for standalone function
    explicit Callback(void (*func)()) : callback_(new StandaloneCallback(func))
    {
    }

    // Constructor for non-const member function
    template <typename T>
    Callback(T* instance, void (T::*func)()) : callback_(new MemberCallback<T>(instance, func))
    {
    }

    // Constructor for const member function
    template <typename T>
    Callback(const T* instance, void (T::*func)() const) : callback_(new ConstMemberCallback<T>(instance, func))
    {
    }

    ~Callback()
    {
        delete callback_;
    }

    void execute() const
    {
        if (callback_)
        {
            callback_->execute();
        }
    }

private:
    CallbackBase* callback_;
};

constexpr uint8_t CALLBACK_POINTER_SIZE = sizeof(Callback*);

#endif //CALLBACK_H
