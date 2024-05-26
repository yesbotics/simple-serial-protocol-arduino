#ifndef MEMBERCALLBACK_H
#define MEMBERCALLBACK_H

#include "CallbackBase.h"

// Member function callback template
template <typename T>
class MemberCallback final : public CallbackBase {
public:
    using FunctionType = void (T::*)();

    MemberCallback(T* instance, FunctionType func) : instance_(instance), func_(func) {}

    void execute() override {
        if (instance_ && func_) {
            (instance_->*func_)();
        }
    }

private:
    T* instance_;
    FunctionType func_;
};

#endif //MEMBERCALLBACK_H
