#ifndef CONSTMEMBERFUNCTION_H
#define CONSTMEMBERFUNCTION_H

#include "CallbackBase.h"
#include "functypes.h"

// Const member function callback template
template <typename T>
class ConstMemberCallback final : public CallbackBase
{
public:
    ConstMemberCallback(
        const T* instance,
        const FunctionTypeConstMember<T> func
    ) :
        instance_(instance),
        func_(func),
        func2_(nullptr)
    {
    }

    ConstMemberCallback(
        const T* instance,
        const FunctionTypeConstMemberWithUint8Arg<T> func
    ) :
        instance_(instance),
        func_(nullptr),
        func2_(func)
    {
    }

    void execute() override
    {
        if (instance_ != nullptr && func_ != nullptr)
        {
            (instance_->*func_)();
        }
    }

    void execute(uint8_t num) override
    {
        if (instance_ != nullptr && func2_ != nullptr)
        {
            (instance_->*func2_)(num);
        }
    }

private:
    const T* instance_;
    FunctionTypeConstMember<T> func_;
    FunctionTypeConstMemberWithUint8Arg<T> func2_;
};

#endif //CONSTMEMBERFUNCTION_H
