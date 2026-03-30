cmake_minimum_required(VERSION 3.10)
project(NoitaEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ядро движка (Библиотека)
add_library(noita_core SHARED
    core/noita_core.cpp
    core/noita_core.hpp
)

target_include_directories(noita_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/core)

# Настройки экспорта символов
target_compile_definitions(noita_core PRIVATE NOITA_CORE_EXPORTS)

# Настройки десктопного приложения
find_package(raylib QUIET)

if(NOT raylib_FOUND)
    message(STATUS "Raylib not found locally, assuming it will be provided or compiled manually.")
endif()

add_executable(noita_desktop platforms/desktop/main.cpp)
target_link_libraries(noita_desktop PRIVATE noita_core raylib)
target_include_directories(noita_desktop PRIVATE core)
