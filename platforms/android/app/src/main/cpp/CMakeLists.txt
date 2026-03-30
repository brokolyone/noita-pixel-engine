cmake_minimum_required(VERSION 3.10)
project(NoitaEngineAndroid)

set(CMAKE_CXX_STANDARD 17)

# Исходники ядра
set(CORE_SOURCES
    ../../../../../../core/noita_core.cpp
    ../../../../../../core/noita_core.hpp
)

# Исходники приложения
set(APP_SOURCES
    ../../../../../../platforms/desktop/main.cpp
)

# Подключаем Raylib
add_subdirectory(raylib)

add_library(noita_engine_android SHARED
    ${CORE_SOURCES}
    ${APP_SOURCES}
)

target_include_directories(noita_engine_android PRIVATE
    ../../../../../../core
    raylib/src
)

# Необходимые системные библиотеки Android
target_link_libraries(noita_engine_android
    raylib
    android
    log
    EGL
    GLESv2
)

# Для работы NativeActivity
target_compile_definitions(noita_engine_android PRIVATE PLATFORM_ANDROID)
