cmake_minimum_required(VERSION 3.22)

project(ClientBase VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Release)
set(BUILD_SHARED_LIBS ON)
set(CMAKE_DISABLE_TESTING ON)
set(CMAKE_CXX_STANDARD 17)

include_directories($ENV{JAVA_HOME}/include)
include_directories($ENV{JAVA_HOME}/include/win32)
link_directories($ENV{JAVA_HOME}/lib)

include_directories(loader/include)

add_library(loader
    loader/dllmain.cpp
    loader/src/class_loader.cpp
    loader/src/mapper.cpp
    loader/src/jni_natives.cpp
    loader/src/main_natives.cpp
    loader/src/utils.cpp
)

set_target_properties(loader PROPERTIES
    PREFIX ""
)

target_include_directories(loader PUBLIC
    ${JAVA_HOME}/include
)

target_link_libraries(loader PUBLIC -static -static-libgcc -static-libstdc++
    Jvm
)

add_executable(inject
    injector/inject.cpp
)

target_link_libraries(inject PUBLIC -static -static-libgcc -static-libstdc++
)
