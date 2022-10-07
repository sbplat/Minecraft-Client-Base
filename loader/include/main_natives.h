#ifndef MAIN_NATIVES_H
#define MAIN_NATIVES_H

#include <jni.h>
#include <jvmti.h>

namespace MainNatives {

struct ClassData {
    jint length;
    unsigned char *bytes;

    ~ClassData();
};

// Enables the ClassFileLoadHook callback
// targetClassSig: The signature of the class to hook.
// classData: This will be filled with the class data.
jvmtiError EnableClassFileLoadHook(JNIEnv *jniEnv, char *targetClassSig, ClassData *classData);

// Disables the ClassFileLoadHook callback
jvmtiError DisableClassFileLoadHook(JNIEnv *jniEnv);

// Gets the bytes of the class
// clazz: The class to get the bytes of.
jbyteArray GetClassBytes(JNIEnv *jniEnv, jobject, jclass clazz);

// Sets the bytes of the class
// clazz: The class to set the bytes of.
// classBytes: The bytes to set.
jvmtiError SetClassBytes(JNIEnv *jniEnv, jobject, jclass clazz, jbyteArray classBytes);

// Get the class field as an array of string objects
// clazz: The class to get the fields of.
// fieldNames: If fieldNames is true, the names of the fields will be returned.
//             Otherwise, the descriptors will be returned.
jobjectArray GetClassFields(JNIEnv *jniEnv, jobject, jclass clazz, jboolean fieldNames);

// Get the class method as an array of string objects
// clazz: The class to get the methods of.
// methodNames: If methodNames is true, the names of the methods will be returned.
//              Otherwise, the descriptors will be returned.
jobjectArray GetClassMethods(JNIEnv *jniEnv, jobject, jclass clazz, jboolean methodNames);

// Register the native functions
// clazz: The class to register the native functions to.
void RegisterMainNatives(JNIEnv *jniEnv, jclass clazz);

}  // namespace MainNatives

#endif  // MAIN_NATIVES_H
