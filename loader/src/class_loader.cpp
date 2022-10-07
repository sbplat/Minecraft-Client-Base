#include "class_loader.h"

#include "utils.h"

ClassLoader::ClassLoader() {
}

ClassLoader::~ClassLoader() {
    ctx.env->DeleteGlobalRef(classLoaderObject);
}

void ClassLoader::SetMinecraftClient(CLIENT client) {
    Client = client;
}

void ClassLoader::InitializeClassLoaderObject() {
    if (classLoaderObject != NULL) {
        return;
    }

    if (Client == CLIENT::VANILLA) {
        // Only used for DefineClass
        jclass classLoaderClass = ctx.env->FindClass("java/lang/ClassLoader");
        jmethodID getSystemClassLoaderMethod = ctx.env->GetStaticMethodID(classLoaderClass, "getSystemClassLoader", "()Ljava/lang/ClassLoader;");

        jobject classLoaderObjectLocal = ctx.env->CallStaticObjectMethod(classLoaderClass, getSystemClassLoaderMethod);
        classLoaderObject = ctx.env->NewGlobalRef(classLoaderObjectLocal);
        ctx.env->DeleteLocalRef(classLoaderObjectLocal);

    } else if (Client == CLIENT::FORGE) {
        jclass launchClass = ctx.env->FindClass("net/minecraft/launchwrapper/Launch");

        // Get the LaunchClassLoader object
        jfieldID classLoaderField = ctx.env->GetStaticFieldID(launchClass, "classLoader", "Lnet/minecraft/launchwrapper/LaunchClassLoader;");

        jobject classLoaderObjectLocal = ctx.env->GetStaticObjectField(launchClass, classLoaderField);
        classLoaderObject = ctx.env->NewGlobalRef(classLoaderObjectLocal);
        ctx.env->DeleteLocalRef(classLoaderObjectLocal);

    } else if (Client == CLIENT::FORGE_ESSENTIAL) {
        // Find an instance of the RelaunchClassLoader class
        jclass relaunchClassLoaderClass = ctx.env->FindClass("gg/essential/loader/stage2/relaunch/RelaunchClassLoader");

        auto heap_object_callback = [](jlong class_tag, jlong size, jlong *tag_ptr, void *user_data) {
            *tag_ptr = 1337;
            return JVMTI_ITERATION_ABORT;  // We only need one instance
        };

        ctx.jvmti->IterateOverInstancesOfClass(relaunchClassLoaderClass, JVMTI_HEAP_OBJECT_EITHER, heap_object_callback, NULL);

        jlong tag = 1337;
        jint count;
        jobject *instances;
        ctx.jvmti->GetObjectsWithTags(1, &tag, &count, &instances, NULL);

        // Get the instance
        jobject relaunchClassLoaderObject = instances[0];
        ctx.jvmti->Deallocate((unsigned char *)instances);

        // Get the RelaunchClassLoader findClass method
        jmethodID relaunchFindClassMethod = ctx.env->GetMethodID(relaunchClassLoaderClass, "findClass", "(Ljava/lang/String;)Ljava/lang/Class;");

        jstring launchClassString = ctx.env->NewStringUTF("net.minecraft.launchwrapper.Launch");
        jclass launchClass = (jclass)ctx.env->CallObjectMethod(relaunchClassLoaderObject, relaunchFindClassMethod, launchClassString);

        // Get the LaunchClassLoader object
        jfieldID classLoaderField = ctx.env->GetStaticFieldID(launchClass, "classLoader", "Lnet/minecraft/launchwrapper/LaunchClassLoader;");

        jobject classLoaderObjectLocal = ctx.env->GetStaticObjectField(launchClass, classLoaderField);
        classLoaderObject = ctx.env->NewGlobalRef(classLoaderObjectLocal);
        ctx.env->DeleteLocalRef(classLoaderObjectLocal);
    }
}

void ClassLoader::InitializeFindClassMethod() {
    if (findClassMethod != NULL) {
        return;
    }

    if (Client == CLIENT::VANILLA) {
        return;
    }

    jclass launchClassLoaderClass = ctx.env->GetObjectClass(classLoaderObject);
    findClassMethod = ctx.env->GetMethodID(launchClassLoaderClass, "findClass", "(Ljava/lang/String;)Ljava/lang/Class;");
}

jclass ClassLoader::FindClass(JNIEnv *jniEnv, std::string seargeClassName) {
    std::string clientClassName = mapper.SeargeToClientClass(seargeClassName);

    if (Client == CLIENT::VANILLA) {
        return jniEnv->FindClass(clientClassName.c_str());
    }

    InitializeClassLoaderObject();
    InitializeFindClassMethod();

    jstring seargeClassNameJStr = jniEnv->NewStringUTF(clientClassName.c_str());

    return (jclass)jniEnv->CallObjectMethod(classLoaderObject, findClassMethod, seargeClassNameJStr);
}

jclass ClassLoader::FindClass(std::string seargeClassName) {
    return FindClass(ctx.env, seargeClassName);
}

jclass ClassLoader::LoadClass(std::string fullClassName, std::string classPath) {
    InitializeClassLoaderObject();

    std::string classData;
    mapper.Load(getParentPath() + classPath, classData, true);
    return ctx.env->DefineClass(fullClassName.c_str(), classLoaderObject, (jbyte *)classData.c_str(), classData.size());
}

std::map<std::string, jclass> ClassLoader::LoadClient(std::map<std::string, std::string> classPaths) {
    std::map<std::string, jclass> loadedClasses;

    for (auto [fullClassName, classPath] : classPaths) {
        size_t lastSlash = fullClassName.find_last_of('/');
        std::string className = fullClassName.substr(lastSlash + 1);
        loadedClasses[className] = LoadClass(fullClassName, classPath);
    }

    return loadedClasses;
}

std::map<std::string, jclass> ClassLoader::LoadJar(std::string jarPath) {
    std::map<std::string, std::string> classPaths;

    // TODO: implement method to get all classes in a jar

    return LoadClient(classPaths);
}

ClassLoader classLoader;
