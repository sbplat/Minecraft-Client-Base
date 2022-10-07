#ifndef NATIVES_H
#define NATIVES_H

#include <jni.h>

namespace JniNatives {

// Get the mapped (obfuscated) class name
// seargeClassName: The Searge name of the class to get the mapped name of.
jstring GetClassMapped(JNIEnv *jniEnv, jobject, jstring seargeClassName);

// Finds and returns the class with the given Searge name
// seargeClassName: The Searge name of the class to find.
jclass FindClass(JNIEnv *jniEnv, jobject, jstring seargeClassName);

// Get the mapped (obfuscated) method name
// seargeClassName: The Searge name of the class the method is in.
// seargeMethodName: The Searge name of the method to get the mapped name of.
// seargeMethodDescriptor: The Searge descriptor of the method to get the mapped name of.
jstring GetMethodMapped(JNIEnv *jniEnv, jobject, jstring seargeClassName, jstring seargeMethodName, jstring seargeMethodDescriptor);

// Get the mapped (obfuscated) field name
// seargeClassName: The Searge name of the class the field is in.
// seargeFieldName: The Searge name of the field to get the mapped name of.
jstring GetFieldMapped(JNIEnv *jniEnv, jobject, jstring seargeClassName, jstring seargeFieldName);

// Register the native functions
// clazz: The class to register the native functions to.
void RegisterJniNatives(JNIEnv *jniEnv, jclass clazz);

}  // namespace JniNatives

#endif  // NATIVES_H
