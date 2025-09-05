cmake_minimum_required(VERSION 3.16)
project(bardzo_mini_SCADA_wrzesien VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 REQUIRED COMPONENTS Core Gui Widgets Charts)

message(STATUS "Qt6_DIR = ${Qt6_DIR}")
message(STATUS "Qt6 version = ${Qt6_VERSION}")

set(SOURCES
        main.cpp
        OknoGlowne.cpp
)
set(HEADERS
        OknoGlowne.hpp
        Dane.hpp
        Typy.hpp
        ZbiorDanych.hpp
        WykresLiniowy.hpp
        Wyswietlacz.hpp
)

qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Charts)
qt_finalize_executable(${PROJECT_NAME})
