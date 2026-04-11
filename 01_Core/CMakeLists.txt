cmake_minimum_required(VERSION 3.16)
project(SSOEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DPLATFORM_DESKTOP)

file(GLOB_RECURSE ALL_SOURCES "*.cpp")
list(FILTER ALL_SOURCES EXCLUDE REGEX "build/")

set(CORE_SOURCES "")

if(ANDROID)
    foreach(src ${ALL_SOURCES})
        if(src MATCHES "android/" OR src MATCHES "main.cpp")
            list(APPEND CORE_SOURCES ${src})
        endif()
    endforeach()
    
    # Find the glue source in NDK
    set(ANDROID_GLUE_PATH "${CMAKE_ANDROID_NDK}/sources/android/native_app_glue")
    include_directories(${ANDROID_GLUE_PATH})
    
    # Add the glue .c file to your sources
    set(CORE_SOURCES ${CORE_SOURCES} "${ANDROID_GLUE_PATH}/android_native_app_glue.c")
    
    add_library(SSOEngine SHARED ${CORE_SOURCES})
    
    target_include_directories(SSOEngine PRIVATE 
        "../include"
        "include" 
        "tools" 
        "."
        ${ANDROID_GLUE_PATH}
    )
    
    # Path to your Android Raylib library
    set(RAYLIB_ANDROID_LIB "${CMAKE_CURRENT_SOURCE_DIR}/../lib/android/arm64-v8a/libraylib.a")
    
    target_link_libraries(SSOEngine 
        ${RAYLIB_ANDROID_LIB}
        android 
        log 
        EGL 
        GLESv2
    )
    
    target_compile_definitions(SSOEngine PRIVATE PLATFORM_ANDROID)
    set_target_properties(SSOEngine PROPERTIES ANDROID_STL c++_static)
else()
    foreach(src ${ALL_SOURCES})
        if(NOT src MATCHES "android/")
            list(APPEND CORE_SOURCES ${src})
        endif()
    endforeach()
    
    if(WIN32)
        set(WINDOWS_SOURCES ${CORE_SOURCES} resource.rc)
    else()
        set(WINDOWS_SOURCES ${CORE_SOURCES})
    endif()
    add_executable(SSOEngine ${WINDOWS_SOURCES})
    target_compile_definitions(SSOEngine PRIVATE PLATFORM_DESKTOP)
    
    target_include_directories(SSOEngine PRIVATE 
        "../include"
        "include" 
        "tools" 
        "."
    )
    
    target_link_directories(SSOEngine PRIVATE ../lib)
    
    target_link_libraries(SSOEngine 
        raylib 
        opengl32 
        gdi32 
        winmm
    )
endif()