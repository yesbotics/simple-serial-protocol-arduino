#ifndef CALLBACK_H
#define CALLBACK_H

#include <Arduino.h>

#include "StandaloneCallback.h"
#include "MemberCallback.h"
#include "ConstMemberCallback.h"
#include "functypes.h"

// Wrapper class for callbacks
class Callback
{
public:
    // Constructor for standalone function
    explicit Callback(
        const FunctionTypeStandalone func) :
        callback_(
            new StandaloneCallback(func)
        )
    {
    }

    // Constructor for non-const member function
    template <typename T>
    Callback(
        T* instance,
        FunctionTypeMember<T> func
    ) :
        callback_(
            new MemberCallback<T>(instance, func)
        )
    {
    }

    // Constructor for const member function
    template <typename T>
    Callback(
        const T* instance,
        FunctionTypeConstMember<T> func
    ) :
        callback_(new ConstMemberCallback<T>(instance, func))
    {
    }

    // Constructor for standalone function
    explicit Callback(
        const FunctionTypeStandaloneWithUint8Arg func) :
        callback_(
            new StandaloneCallback(func)
        )
    {
    }

    // Constructor for non-const member function
    template <typename T>
    Callback(
        T* instance,
        FunctionTypeMemberWithUint8Arg<T> func
    ) :
        callback_(
            new MemberCallback<T>(instance, func)
        )
    {
    }

    // Constructor for const member function
    template <typename T>
    Callback(
        const T* instance,
        FunctionTypeConstMemberWithUint8Arg<T> func
    ) :
        callback_(new ConstMemberCallback<T>(instance, func))
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

    void execute(const uint8_t num) const
    {
        if (callback_)
        {
            callback_->execute(num);
        }
    }

private:
    CallbackBase* callback_;
};

constexpr uint8_t CALLBACK_POINTER_SIZE = sizeof(Callback*);

#endif //CALLBACK_H
