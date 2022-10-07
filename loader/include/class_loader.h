#ifndef CLASS_LOADER_H
#define CLASS_LOADER_H

#include "mapper.h"

#include <jni.h>

#include <map>
#include <string>

class ClassLoader {
private:
    CLIENT Client;

    jobject classLoaderObject = NULL;

    jmethodID findClassMethod = NULL;

    void InitializeClassLoaderObject();
    void InitializeFindClassMethod();

public:
    ClassLoader();
    ~ClassLoader();

    // Set the Minecraft client for automatic mapping
    // client: The Minecraft client to set.
    void SetMinecraftClient(CLIENT client);

    // Finds a class given the Searge class name
    // seargeClassName: The Searge class name of the class to find.
    jclass FindClass(JNIEnv *jniEnv, std::string seargeClassName);

    // Finds a class given the Searge class name
    // seargeClassName: The Searge class name of the class to find.
    jclass FindClass(std::string seargeClassName);

    // Loads a class given the class name and the class path
    // fullClassName: The full class name.
    // classPath: The class path relative to the dll parent directory.
    jclass LoadClass(std::string fullClassName, std::string classPath);

    // Loads the client classes given the class paths
    // classPaths: A map of the class names to the class paths.
    std::map<std::string, jclass> LoadClient(std::map<std::string, std::string> classPaths);

    // Loads a jar given the jar path
    // jarPath: The path to the jar.
    // TODO: Finish implementation
    std::map<std::string, jclass> LoadJar(std::string jarPath);
};

extern ClassLoader classLoader;

#endif  // CLASS_LOADER_H
