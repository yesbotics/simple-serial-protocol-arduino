#ifdef MAIN_SSP3

#include <Arduino.h>
#include <callback/Callback.h>


// Standalone function
void standaloneFunction()
{
    Serial.println("Standalone function called");
}

// Standalone function with num
void standaloneFunctionWithNum(const uint8_t num)
{
    Serial.print("Standalone function with num called: ");
    Serial.println(num);
}

// Class with a member function
class MyClass
{
public:
    explicit MyClass(const String& name): name_{name} // NOLINT(*-pass-by-value)
    {
    }

    static void staticClassFunction()
    {
        Serial.println("staticClassFunction()");
    }

    static void staticClassFunctionWithNum(const uint8_t num)
    {
        Serial.print("staticClassFunctionWithNum(): ");
        Serial.println(num);
    }

    // ReSharper disable once CppMemberFunctionMayBeConst
    void nonConstMemberFunction()
    {
        Serial.print("nonConstMemberFunction(): ");
        Serial.println(name_);
    }

    // ReSharper disable once CppMemberFunctionMayBeConst
    void nonConstMemberFunctionWithNum(const uint8_t num)
    {
        Serial.print("nonConstMemberFunctionWithNum(): ");
        Serial.print(name_);
        Serial.print(", ");
        Serial.println(num);
    }

    void constMemberFunction() const
    {
        Serial.print("constMemberFunction(): ");
        Serial.println(name_);
    }

    void constMemberFunctionWithNum(const uint8_t num) const
    {
        Serial.print("constMemberFunctionWithNum(): ");
        Serial.print(name_);
        Serial.print(", ");
        Serial.println(num);
    }

private:
    String name_;
};

void setup()
{
    Serial.begin(115200);

    while (!Serial)
    {
    };

    Serial.println();

    MyClass nonConstInstance{"nonConstInstance"};
    const MyClass constInstance{"constInstance"};

    const Callback cbStaticClassFct(MyClass::staticClassFunction);
    cbStaticClassFct.execute();

    const Callback cbStaticClassFctWithNum(MyClass::staticClassFunctionWithNum);
    cbStaticClassFctWithNum.execute(1);

    const Callback cbNonConstInstance_nonConstMemberFunction(&nonConstInstance, &MyClass::nonConstMemberFunction);
    cbNonConstInstance_nonConstMemberFunction.execute();

    const Callback cbNonConstInstance_nonConstMemberFunctionWithNum(
        &nonConstInstance,
        &MyClass::nonConstMemberFunctionWithNum
    );
    cbNonConstInstance_nonConstMemberFunctionWithNum.execute(2);

    const Callback cbNonConstInstance_constMemberFunction(&nonConstInstance, &MyClass::constMemberFunction);
    cbNonConstInstance_constMemberFunction.execute();

    const Callback cbNonConstInstance_constMemberFunctionWithNum(
        &nonConstInstance,
        &MyClass::constMemberFunctionWithNum
    );
    cbNonConstInstance_constMemberFunctionWithNum.execute(3);


    const Callback cbConstInstance_nonConstMemberFunction(&constInstance, &MyClass::nonConstMemberFunction);
    cbConstInstance_nonConstMemberFunction.execute();

    const Callback cbConstInstance_nonConstMemberFunctionWithNum(
        &constInstance,
        &MyClass::nonConstMemberFunctionWithNum
    );
    cbConstInstance_nonConstMemberFunctionWithNum.execute(2);

    const Callback cbConstInstance_constMemberFunction(&constInstance, &MyClass::constMemberFunction);
    cbConstInstance_constMemberFunction.execute();

    const Callback cbConstInstance_constMemberFunctionWithNum(
        &constInstance,
        &MyClass::constMemberFunctionWithNum
    );
    cbConstInstance_constMemberFunctionWithNum.execute(3);
}

void loop()
{
    // Empty loop
}


#endif // MAIN_SSP3