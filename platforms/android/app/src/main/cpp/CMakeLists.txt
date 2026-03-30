cmake_minimum_required(VERSION 3.10)
project(NoitaEngineAndroid)

set(CMAKE_CXX_STANDARD 17)

# Исходники ядра (из корня проекта)
set(CORE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../core/noita_core.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../core/noita_core.hpp
)

# Исходники приложения (из папки desktop)
set(APP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../platforms/desktop/main.cpp
)

# Подключаем Raylib
add_subdirectory(raylib)

add_library(noita_engine_android SHARED
    ${CORE_SOURCES}
    ${APP_SOURCES}
)

target_include_directories(noita_engine_android PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../core
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib/src
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
