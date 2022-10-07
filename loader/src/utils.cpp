#include "utils.h"

#include "class_loader.h"
#include "mapper.h"

#include <windows.h>

#include <filesystem>
#include <iostream>

JVM_CTX ctx;

std::string jstringToString(JNIEnv *jniEnv, jstring str) {
    if (!str) {
        return std::string();
    }

    int length = jniEnv->GetStringUTFLength(str);
    const char *data = jniEnv->GetStringUTFChars(str, 0);
    std::string string(data, length);
    jniEnv->ReleaseStringUTFChars(str, data);
    return string;
}

std::string jstringToString(jstring str) {
    return jstringToString(ctx.env, str);
}

void setClient() {
    jclass MinecraftForgeClass = ctx.env->FindClass("net/minecraftforge/common/MinecraftForge");
    JNI_EXCEPTION_CHECK

    if (MinecraftForgeClass != NULL) {
        jclass EssentialModClass = ctx.env->FindClass("gg/essential/loader/stage2/relaunch/RelaunchClassLoader");
        JNI_EXCEPTION_CHECK

        if (EssentialModClass != NULL) {
            std::cout << "Client is: "
                      << "FORGE_ESSENTIAL" << std::endl;
            mapper.SetClient(CLIENT::FORGE_ESSENTIAL);

        } else {
            std::cout << "Client is: "
                      << "FORGE" << std::endl;
            mapper.SetClient(CLIENT::FORGE);
        }

    } else {
        std::cout << "Client is: "
                  << "VANILLA" << std::endl;
        mapper.SetClient(CLIENT::VANILLA);
    }

    classLoader.SetMinecraftClient(mapper.GetClient());

    ctx.env->DeleteLocalRef(MinecraftForgeClass);
}

std::string _dllPath;

std::string getDllPath() {
    if (_dllPath.size() != 0) {
        return _dllPath;
    }

    char dllPathBuffer[MAX_PATH];
    HMODULE hModule = NULL;

    GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&getDllPath, &hModule);
    GetModuleFileName(hModule, dllPathBuffer, MAX_PATH);

    return _dllPath = std::string(dllPathBuffer);
}

std::string _parentPath;

std::string getParentPath() {
    if (_parentPath.size() != 0) {
        return _parentPath;
    }

    std::filesystem::path dllPath(getDllPath());  // ./build/loader.dll

    return _parentPath = dllPath
        .parent_path()  // ./build
        .parent_path()  // ./
        .string();
}
