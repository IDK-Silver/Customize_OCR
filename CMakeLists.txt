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

set(Qt_Widgets
        src/widget/main_window/main_window.ui
        src/widget/main_window/main_window.cpp
        src/widget/main_window/main_window.h

        src/widget/ocr_detect_widget/ocr_detect_widget.ui
        src/widget/ocr_detect_widget/ocr_detect_widget.cpp
        src/widget/ocr_detect_widget/ocr_detect_widget.h
        src/widget/ocr_detect_widget/widget_item/ocr_data_widget.ui
        src/widget/ocr_detect_widget/widget_item/ocr_data_widget.cpp
        src/widget/ocr_detect_widget/widget_item/ocr_data_widget.h
        src/widget/ocr_detect_widget/data_struct/ocr_data_struct.cpp

        src/widget/ocr_format_create_widget/ocr_format_create_widget.ui
        src/widget/ocr_format_create_widget/ocr_format_create_widget.cpp
        src/widget/ocr_format_create_widget/ocr_format_create_widget.h
        )

set(Lib_Config
        lib/Config/Files_Path/Files_Path.cpp
        lib/Config/Files_Path/Files_Path.h
        lib/Config/Global_Config/Global_Config.cpp
        lib/Config/Global_Config/Global_Config.h)

set(Lib_Utility
        lib/Utility/Files_Manager/Files_Manager.cpp lib/Utility/Files_Manager/Files_Manager.h)

set(PROJECT_SOURCES
        src/main.cpp
        ${Qt_Widgets}
        ${Lib_Config}
        ${Lib_Utility}
        )

add_executable(PDF_Tool ${PROJECT_SOURCES})
target_link_libraries(PDF_Tool PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)


