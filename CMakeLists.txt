cmake_minimum_required(VERSION 3.22)

project(ClientBase VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Release)
set(BUILD_SHARED_LIBS ON)
set(CMAKE_DISABLE_TESTING ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}>)

find_package(JNI REQUIRED)

include_directories(${JNI_INCLUDE_DIRS})

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

target_link_libraries(loader PUBLIC -static
    ${JNI_LIBRARIES}
)

add_executable(inject
    injector/inject.cpp
)

target_link_libraries(inject PUBLIC -static
)
