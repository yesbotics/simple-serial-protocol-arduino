
#ifndef FUNCTYPES_H
#define FUNCTYPES_H

using FunctionTypeStandalone = void (*)();
template <typename T>
using FunctionTypeMember = void (T::*)();
template <typename T>
using FunctionTypeConstMember =  void (T::*)() const;

using FunctionTypeStandaloneWithUint8Arg = void (*)(uint8_t);
template <typename T>
using FunctionTypeMemberWithUint8Arg =  void (T::*)(uint8_t);
template <typename T>
using FunctionTypeConstMemberWithUint8Arg =  void (T::*)(uint8_t) const;


#endif //FUNCTYPES_H
