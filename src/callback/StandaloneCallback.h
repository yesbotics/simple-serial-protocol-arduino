#ifndef STANDALONECALLBACK_H
#define STANDALONECALLBACK_H

#include "CallbackBase.h"
#include "functypes.h"

// Standalone function callback
class StandaloneCallback final : public CallbackBase
{
public:
    explicit StandaloneCallback(
        const FunctionTypeStandalone func
    ) :
        func_(func),
        func2_(nullptr)
    {
    }

    explicit StandaloneCallback(
        const FunctionTypeStandaloneWithUint8Arg func
    ) :
        func_(nullptr),
        func2_(func)
    {
    }

    void execute() override
    {
        if (func_)
        {
            func_();
        }
    }

    void execute(const uint8_t num) override
    {
        if (func2_)
        {
            func2_(num);
        }
    }

private:
    FunctionTypeStandalone func_;
    FunctionTypeStandaloneWithUint8Arg func2_;
};

#endif //STANDALONECALLBACK_H
