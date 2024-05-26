#ifndef CALLBACKBASE_H
#define CALLBACKBASE_H

// Base class for callbacks
class CallbackBase {
public:
    virtual ~CallbackBase() = default;
    virtual void execute() = 0;
};


#endif //CALLBACKBASE_H
