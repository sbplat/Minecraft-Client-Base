#ifndef UTILS_H
#define UTILS_H

#include <jni.h>
#include <jvmti.h>

#include <string>

struct JVM_CTX {
    JNIEnv *env;
    JavaVM *jvm;
    JavaVMInitArgs vm_args;
    jvmtiEnv *jvmti;
};

extern JVM_CTX ctx;

#define JNI_EXCEPTION_CHECK               \
    {                                     \
        if (ctx.env->ExceptionCheck()) {  \
            ctx.env->ExceptionDescribe(); \
            ctx.env->ExceptionClear();    \
        }                                 \
    }

// Converts a jstring to std::string
// str: The jstring to convert.
std::string jstringToString(JNIEnv *jniEnv, jstring str);
std::string jstringToString(jstring str);

// Sets the current Minecraft client by checking if certain classes exist
void setClient();

extern std::string _dllPath;
std::string getDllPath();

extern std::string _parentPath;
std::string getParentPath();

#endif  // UTILS_H
