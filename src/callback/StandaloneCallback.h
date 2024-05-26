#ifndef STANDALONECALLBACK_H
#define STANDALONECALLBACK_H

#include "CallbackBase.h"

// Standalone function callback
class StandaloneCallback final : public CallbackBase {
public:
    using FunctionType = void (*)();

    explicit StandaloneCallback(const FunctionType func) : func_(func) {}

    void execute() override {
        if (func_) {
            func_();
        }
    }

private:
    FunctionType func_;
};

#endif //STANDALONECALLBACK_H
