#include "main_natives.h"

#include "utils.h"

//#include <fstream>  // For debugging
#include <cstring>
#include <iostream>
#include <thread>

namespace MainNatives {

ClassData::~ClassData() {
    delete[] bytes;
    bytes = nullptr;
}

thread_local char *targetClassSigGlobal = nullptr;
thread_local ClassData *classDataGlobal = nullptr;

jvmtiError EnableClassFileLoadHook(JNIEnv *jniEnv, char *targetClassSig, ClassData *classData) {
    targetClassSigGlobal = targetClassSig;
    classDataGlobal = classData;

    auto ClassFileLoadHookCallback = [](jvmtiEnv *jvmti_env, JNIEnv *jni_env, jclass class_being_redefined, jobject loader, const char *name,
                                        jobject protection_domain, jint class_data_len, const unsigned char *class_data, jint *new_class_data_len,
                                        unsigned char **new_class_data) {
        char *classSignature;
        ctx.jvmti->GetClassSignature(class_being_redefined, &classSignature, nullptr);

        if (strcmp(classSignature, targetClassSigGlobal) != 0) {
            return;
        }

        ctx.jvmti->Deallocate((unsigned char *)classSignature);

        classDataGlobal->length = class_data_len;
        classDataGlobal->bytes = new unsigned char[class_data_len];

        const unsigned char **actual_class_data = (const unsigned char **)class_data;

        memcpy(classDataGlobal->bytes, actual_class_data, class_data_len);

        /*
        std::ofstream file;
        file.open((getParentPath() + "/classbytes.txt").c_str(), std::ios::out | std::ios::binary);
        file.write((const char*)classDataGlobal->bytes, classDataGlobal->length);
        file.close();
        std::cout << "Wrote to file" << std::endl;
        */
    };

    // Set the callback
    jvmtiEventCallbacks eventCallbacks = {0};
    eventCallbacks.ClassFileLoadHook = ClassFileLoadHookCallback;
    jvmtiError res = ctx.jvmti->SetEventCallbacks(&eventCallbacks, sizeof(eventCallbacks));

    if (res != JVMTI_ERROR_NONE) {
        std::cout << "Couldn't set ClassFileLoadHookCallback" << std::endl;
        return res;
    }

    // Enable the notification for our class file load hook
    res = ctx.jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, NULL);

    if (res != JVMTI_ERROR_NONE) {
        std::cout << "Couldn't set SetEventNotificationMode" << std::endl;
        return res;
    }

    return res;
}

jvmtiError DisableClassFileLoadHook(JNIEnv *jniEnv) {
    ctx.jvmti->SetEventNotificationMode(JVMTI_DISABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, NULL);
    return ctx.jvmti->SetEventCallbacks(nullptr, 0);
}

jbyteArray GetClassBytes(JNIEnv *jniEnv, jobject, jclass clazz) {
    char *classSignature;
    ctx.jvmti->GetClassSignature(clazz, &classSignature, nullptr);

    ClassData classData = {0};

    EnableClassFileLoadHook(jniEnv, classSignature, &classData);
    jclass classes[] = {clazz};
    jvmtiError res = ctx.jvmti->RetransformClasses(1, classes);
    DisableClassFileLoadHook(jniEnv);

    ctx.jvmti->Deallocate((unsigned char *)classSignature);

    // TODO: Check if res is an error

    jbyteArray classBytes = jniEnv->NewByteArray(classData.length);
    jniEnv->SetByteArrayRegion(classBytes, 0, classData.length, (jbyte *)classData.bytes);

    return classBytes;
}

jvmtiError SetClassBytes(JNIEnv *jniEnv, jobject, jclass clazz, jbyteArray classBytes) {
    jsize classBytesLength = jniEnv->GetArrayLength(classBytes);
    jbyte *classBytesJByte = jniEnv->GetByteArrayElements(classBytes, nullptr);

    jvmtiClassDefinition classDefinitions[] = {{clazz, classBytesLength, (unsigned char *)classBytesJByte}};

    jvmtiError res = ctx.jvmti->RedefineClasses(1, classDefinitions);

    return res;
}

jobjectArray GetClassFields(JNIEnv *jniEnv, jobject, jclass clazz, jboolean fieldNames) {
    jint fieldCount;
    jfieldID *fieldIDs;

    jvmtiError res = ctx.jvmti->GetClassFields(clazz, &fieldCount, &fieldIDs);
    if (res != JVMTI_ERROR_NONE) {
        std::cout << "jvmti->GetClassFields error: " << res << std::endl;
        return nullptr;
    }

    jclass stringClass = jniEnv->FindClass("java/lang/String");
    jobjectArray fields = jniEnv->NewObjectArray(fieldCount, stringClass, nullptr);

    for (int i = 0; i < fieldCount; ++i) {
        char *fieldString;

        if (fieldNames) {
            res = ctx.jvmti->GetFieldName(clazz, fieldIDs[i], &fieldString, nullptr, nullptr);
        } else {
            res = ctx.jvmti->GetFieldName(clazz, fieldIDs[i], nullptr, &fieldString, nullptr);
        }

        jstring fieldJString = jniEnv->NewStringUTF(fieldString);

        jniEnv->SetObjectArrayElement(fields, i, fieldJString);

        ctx.jvmti->Deallocate((unsigned char *)fieldString);
        ctx.env->DeleteLocalRef(fieldJString);
    }

    ctx.jvmti->Deallocate((unsigned char *)fieldIDs);

    return fields;
}

jobjectArray GetClassMethods(JNIEnv *jniEnv, jobject, jclass clazz, jboolean methodNames) {
    jint methodCount;
    jmethodID *methodIDs;

    jvmtiError res = ctx.jvmti->GetClassMethods(clazz, &methodCount, &methodIDs);
    if (res != JVMTI_ERROR_NONE) {
        std::cout << "jvmti->GetClassMethods error: " << res << std::endl;
        return nullptr;
    }

    jclass stringClass = jniEnv->FindClass("java/lang/String");
    jobjectArray methods = jniEnv->NewObjectArray(methodCount, stringClass, nullptr);

    for (int i = 0; i < methodCount; ++i) {
        char *methodString;

        if (methodNames) {
            res = ctx.jvmti->GetMethodName(methodIDs[i], &methodString, nullptr, nullptr);
        } else {
            res = ctx.jvmti->GetMethodName(methodIDs[i], nullptr, &methodString, nullptr);
        }

        jstring methodJString = jniEnv->NewStringUTF(methodString);

        jniEnv->SetObjectArrayElement(methods, i, methodJString);

        ctx.jvmti->Deallocate((unsigned char *)methodString);
        ctx.env->DeleteLocalRef(methodJString);
    }

    ctx.jvmti->Deallocate((unsigned char *)methodIDs);

    return methods;
}

void RegisterMainNatives(JNIEnv *jniEnv, jclass clazz) {
#define DEFINE_NATIVE_METHOD(METHOD_NAME, METHOD_SIGNATURE, NATIVE_FUNCTION) {(char *)METHOD_NAME, (char *)METHOD_SIGNATURE, (void *)NATIVE_FUNCTION}

    JNINativeMethod natives[] = {
        DEFINE_NATIVE_METHOD("GetClassBytes", "(Ljava/lang/Class;)[B", GetClassBytes),
        DEFINE_NATIVE_METHOD("SetClassBytes", "(Ljava/lang/Class;[B)I", SetClassBytes),
        DEFINE_NATIVE_METHOD("GetClassFields", "(Ljava/lang/Class;Z)[Ljava/lang/String;", GetClassFields),
        DEFINE_NATIVE_METHOD("GetClassMethods", "(Ljava/lang/Class;Z)[Ljava/lang/String;", GetClassMethods),
    };

#undef DEFINE_NATIVE_METHOD

    jniEnv->RegisterNatives(clazz, natives, sizeof(natives) / sizeof(natives[0]));
}

}  // namespace MainNatives
