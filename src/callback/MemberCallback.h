#ifndef MEMBERCALLBACK_H
#define MEMBERCALLBACK_H

#include "CallbackBase.h"
#include "functypes.h"

// Member function callback template
template <typename T>
class MemberCallback final : public CallbackBase
{
public:
    MemberCallback(
        T* instance,
        const FunctionTypeMember<T> func
    ) :
        instance_(instance),
        func_(func),
        func2_(nullptr)
    {
    }

    MemberCallback(
        T* instance,
        const FunctionTypeMemberWithUint8Arg<T> func
    ) :
        instance_(instance),
        func_(nullptr),
        func2_(func)
    {
    }

    void execute() override
    {
        if (instance_ && func_)
        {
            (instance_->*func_)();
        }
    }

    void execute(uint8_t num) override
    {
        if (instance_ && func2_)
        {
            (instance_->*func2_)(num);
        }
    }

private:
    T* instance_;
    FunctionTypeMember<T> func_;
    FunctionTypeMemberWithUint8Arg<T> func2_;
};

#endif //MEMBERCALLBACK_H
