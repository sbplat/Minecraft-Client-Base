#include "jni_natives.h"

#include "class_loader.h"
#include "mapper.h"
#include "utils.h"

#include <iostream>

namespace JniNatives {

/*
GetVersion,
*/

/*
FindClass,
*/
jstring GetClassMapped(JNIEnv *jniEnv, jobject, jstring seargeClassName) {
    std::string mappedClass = mapper.SeargeToClientClass(jstringToString(jniEnv, seargeClassName));
    return jniEnv->NewStringUTF(mappedClass.c_str());
}

jclass FindClass(JNIEnv *jniEnv, jobject, jstring seargeClassName) {
    return classLoader.FindClass(jniEnv, jstringToString(jniEnv, seargeClassName));
}
/*
GetMethodID,

CallObjectMethod,
CallObjectMethodV,
CallObjectMethodA,
CallBooleanMethod,
CallBooleanMethodV,
CallBooleanMethodA,
CallByteMethod,
CallByteMethodV,
CallByteMethodA,
CallCharMethod,
CallCharMethodV,
CallCharMethodA,
CallShortMethod,
CallShortMethodV,
CallShortMethodA,
CallIntMethod,
CallIntMethodV,
CallIntMethodA,
CallLongMethod,
CallLongMethodV,
CallLongMethodA,
CallFloatMethod,
CallFloatMethodV,
CallFloatMethodA,
CallDoubleMethod,
CallDoubleMethodV,
CallDoubleMethodA,
CallVoidMethod,
CallVoidMethodV,
CallVoidMethodA,
*/

jstring GetMethodMapped(JNIEnv *jniEnv, jobject, jstring seargeClassName, jstring seargeMethodName, jstring seargeMethodDescriptor) {
    std::string mappedMethod = mapper.SeargeToClientMethod(jstringToString(jniEnv, seargeClassName), jstringToString(jniEnv, seargeMethodName),
                                                           jstringToString(jniEnv, seargeMethodDescriptor));
    return jniEnv->NewStringUTF(mappedMethod.c_str());
}

/*
GetStaticMethodID,

CallStaticObjectMethod,
CallStaticObjectMethodV,
CallStaticObjectMethodA,
CallStaticBooleanMethod,
CallStaticBooleanMethodV,
CallStaticBooleanMethodA,
CallStaticByteMethod,
CallStaticByteMethodV,
CallStaticByteMethodA,
CallStaticCharMethod,
CallStaticCharMethodV,
CallStaticCharMethodA,
CallStaticShortMethod,
CallStaticShortMethodV,
CallStaticShortMethodA,
CallStaticIntMethod,
CallStaticIntMethodV,
CallStaticIntMethodA,
CallStaticLongMethod,
CallStaticLongMethodV,
CallStaticLongMethodA,
CallStaticFloatMethod,
CallStaticFloatMethodV,
CallStaticFloatMethodA,
CallStaticDoubleMethod,
CallStaticDoubleMethodV,
CallStaticDoubleMethodA,
CallStaticVoidMethod,
CallStaticVoidMethodV,
CallStaticVoidMethodA,
*/

/*
CallNonvirtualObjectMethod,
CallNonvirtualObjectMethodV,
CallNonvirtualObjectMethodA,
CallNonvirtualBooleanMethod,
CallNonvirtualBooleanMethodV,
CallNonvirtualBooleanMethodA,
CallNonvirtualByteMethod,
CallNonvirtualByteMethodV,
CallNonvirtualByteMethodA,
CallNonvirtualCharMethod,
CallNonvirtualCharMethodV,
CallNonvirtualCharMethodA,
CallNonvirtualShortMethod,
CallNonvirtualShortMethodV,
CallNonvirtualShortMethodA,
CallNonvirtualIntMethod,
CallNonvirtualIntMethodV,
CallNonvirtualIntMethodA,
CallNonvirtualLongMethod,
CallNonvirtualLongMethodV,
CallNonvirtualLongMethodA,
CallNonvirtualFloatMethod,
CallNonvirtualFloatMethodV,
CallNonvirtualFloatMethodA,
CallNonvirtualDoubleMethod,
CallNonvirtualDoubleMethodV,
CallNonvirtualDoubleMethodA,
CallNonvirtualVoidMethod,
CallNonvirtualVoidMethodV,
CallNonvirtualVoidMethodA,
*/

/*
GetFieldID,

GetObjectField,
GetBooleanField,
GetByteField,
GetCharField,
GetShortField,
GetIntField,
GetLongField,
GetFloatField,
GetDoubleField,
SetObjectField,
SetBooleanField,
SetByteField,
SetCharField,
SetShortField,
SetIntField,
SetLongField,
SetFloatField,
SetDoubleField,
*/

jstring GetFieldMapped(JNIEnv *jniEnv, jobject, jstring seargeClassName, jstring seargeFieldName) {
    std::string mappedField = mapper.SeargeToClientField(jstringToString(jniEnv, seargeClassName), jstringToString(jniEnv, seargeFieldName));
    return jniEnv->NewStringUTF(mappedField.c_str());
}


/*
GetStaticFieldID,

GetStaticObjectField,
GetStaticBooleanField,
GetStaticByteField,
GetStaticCharField,
GetStaticShortField,
GetStaticIntField,
GetStaticLongField,
GetStaticFloatField,
GetStaticDoubleField,

SetStaticObjectField,
SetStaticBooleanField,
SetStaticByteField,
SetStaticCharField,
SetStaticShortField,
SetStaticIntField,
SetStaticLongField,
SetStaticFloatField,
SetStaticDoubleField,
*/

void RegisterJniNatives(JNIEnv *jniEnv, jclass clazz) {
#define DEFINE_NATIVE_METHOD(METHOD_NAME, METHOD_SIGNATURE, NATIVE_FUNCTION) {(char *)METHOD_NAME, (char *)METHOD_SIGNATURE, (void *)NATIVE_FUNCTION}

    JNINativeMethod natives[] = {
        DEFINE_NATIVE_METHOD("GetClassMapped", "(Ljava/lang/String;)Ljava/lang/String;", GetClassMapped),
        DEFINE_NATIVE_METHOD("FindClass", "(Ljava/lang/String;)Ljava/lang/Class;", FindClass),
        DEFINE_NATIVE_METHOD("GetMethodMapped", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;", GetMethodMapped),
        DEFINE_NATIVE_METHOD("GetFieldMapped", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;", GetFieldMapped)};

#undef DEFINE_NATIVE_METHOD

    jniEnv->RegisterNatives(clazz, natives, sizeof(natives) / sizeof(natives[0]));
}

}  // namespace JniNatives
