cmake_minimum_required(VERSION 3.10)
project(NoitaEngineAndroid)

set(CMAKE_CXX_STANDARD 17)

# Исходники ядра
set(CORE_SOURCES
    ../../../../core/noita_core.cpp
    ../../../../core/noita_core.hpp
)

# Исходники приложения (можно использовать тот же main.cpp или адаптированный)
# Для Raylib под Android используется NativeActivity, поэтому код почти не меняется
set(APP_SOURCES
    ../../../../platforms/desktop/main.cpp
)

# Подключаем Raylib (Android версия обычно собирается отдельно)
# В идеале здесь мы бы добавили raylib как подпроект или использовали уже скомпилированную версию
# find_package(raylib REQUIRED) # Это требует предустановленного Raylib для Android

add_library(noita_engine_android SHARED
    ${CORE_SOURCES}
    ${APP_SOURCES}
)

target_include_directories(noita_engine_android PRIVATE
    ../../../../core
)

# Необходимые системные библиотеки Android
target_link_libraries(noita_engine_android
    android
    log
    EGL
    GLESv2
    # raylib # Нужно будет добавить скомпилированную библиотеку raylib
)

# Для работы NativeActivity
target_compile_definitions(noita_engine_android PRIVATE PLATFORM_ANDROID)
