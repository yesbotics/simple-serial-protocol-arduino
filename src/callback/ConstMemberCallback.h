#ifndef CONSTMEMBERFUNCTION_H
#define CONSTMEMBERFUNCTION_H

#include "CallbackBase.h"

// Const member function callback template
template <typename T>
class ConstMemberCallback : public CallbackBase {
public:
    using FunctionType = void (T::*)() const;

    ConstMemberCallback(const T* instance, FunctionType func) : instance_(instance), func_(func) {}

    void execute() override {
        if (instance_ && func_) {
            (instance_->*func_)();
        }
    }

private:
    const T* instance_;
    FunctionType func_;
};

#endif //CONSTMEMBERFUNCTION_H
