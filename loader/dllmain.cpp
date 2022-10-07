#include "class_loader.h"
#include "jni_natives.h"
#include "main_natives.h"
#include "utils.h"

#include <jni.h>
#include <jvmti.h>
#include <windows.h>

#include <iostream>
#include <map>
#include <string>

void init_thread(HMODULE hModule) {
    jsize jvmCount;

    if (JNI_GetCreatedJavaVMs(&ctx.jvm, 1, &jvmCount) != JNI_OK || jvmCount == 0) {
        std::cout << "JVM not found (JNI)" << std::endl;
        return;
    }

    jint res = ctx.jvm->GetEnv((void **)&ctx.env, JNI_VERSION_1_6);

    if (res == JNI_EDETACHED) {
        res = ctx.jvm->AttachCurrentThread((void **)&ctx.env, nullptr);
    }

    if (res != JNI_OK) {
        std::cout << "Couldn't attach thread (JNI)" << std::endl;
        return;
    }

    res = ctx.jvm->GetEnv((void **)&ctx.jvmti, JVMTI_VERSION_1_2);

    if (res == JNI_EDETACHED) {
        res = ctx.jvm->AttachCurrentThread((void **)&ctx.jvmti, nullptr);
    }

    if (res != JNI_OK) {
        std::cout << "Couldn't attach thread (JVMTI)" << std::endl;
        return;
    }

    jvmtiCapabilities tiCapabilities = {0};
    tiCapabilities.can_tag_objects = 1;
    tiCapabilities.can_retransform_classes = 1;
    tiCapabilities.can_retransform_any_class = 1;
    tiCapabilities.can_redefine_classes = 1;
    tiCapabilities.can_redefine_any_class = 1;
    tiCapabilities.can_generate_all_class_hook_events = 1;
    ctx.jvmti->AddCapabilities(&tiCapabilities);

    std::cout << "Loader initialized!" << std::endl;

    setClient();  // Set the client and initialize the mapper

    std::map<std::string, std::string> classPaths = {{"inject/client/Client", "/client/Client.class"}};

    std::map<std::string, jclass> loadedClasses = classLoader.LoadClient(classPaths);

    jclass clientClass = loadedClasses["Client"];

    JniNatives::RegisterJniNatives(ctx.env, clientClass);
    MainNatives::RegisterMainNatives(ctx.env, clientClass);

    jmethodID testMethod = ctx.env->GetStaticMethodID(clientClass, "test", "(I)V");
    ctx.env->CallStaticObjectMethod(clientClass, testMethod, 3);

    Sleep(5000);  // Wait a little bit before detaching the loader (native methods are still in use)

    std::cout << "Detaching loader" << std::endl;
    ctx.jvm->DetachCurrentThread();
    FreeLibraryAndExitThread(hModule, EXIT_SUCCESS);
    return;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
    switch (dwReason) {
        case DLL_PROCESS_ATTACH:
            CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)init_thread, hModule, 0, nullptr));

        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}
