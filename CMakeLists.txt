# Basic Setting
cmake_minimum_required(VERSION 3.5)

project(Customize_OCR VERSION 1.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Find Qt Lib
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)


set(PROJECT_SOURCES
        src/main.cpp
        )

add_executable(PDF_Tool ${PROJECT_SOURCES})
target_link_libraries(PDF_Tool PRIVATE Qt${QT_VERSION_MAJOR}::Widgets poppler-qt5 poppler-cpp)


