# Basic Setting
cmake_minimum_required(VERSION 3.5)

project(Customize_OCR VERSION 1.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set path the exe run directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# using conan package manager and run conan setup
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# find Qt lib
find_package(QT NAMES Qt6 COMPONENTS Widgets REQUIRED)
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
        src/widget/image_view_widget/image_view_widget.cpp src/widget/image_view_widget/image_view_widget.h
        src/widget/ocr_format_create_widget/data_struct/ocr_format_struct.cpp
        src/widget/ocr_format_create_widget/data_struct/ocr_format_struct.h src/widget/main_window/strcut/action_enum.h)

set(Lib_Config
        lib/Config/Files_Path/Files_Path.cpp
        lib/Config/Files_Path/Files_Path.h
        lib/Config/Global_Config/Global_Config.cpp
        lib/Config/Global_Config/Global_Config.h)

set(Lib_Utility
        lib/Utility/Files_Manager/Files_Manager.cpp)


set(Lib_OCR
        lib/OCR/ImageOCR.cpp lib/OCR/ImageOCR.h)

set(Lib_ImageFormat
        lib/Utility/ImageFormat/ImageFormat.cpp
        lib/Utility/ImageFormat/ImageFormat.h)

set(PROJECT_SOURCES
        src/main.cpp
        ${Lib_Config}
        ${Qt_Widgets}
        ${Lib_Utility}
        ${Lib_OCR}
        ${Lib_ImageFormat}
         )

add_executable(Customize_OCR ${PROJECT_SOURCES})
target_link_libraries(Customize_OCR PRIVATE ${CONAN_LIBS} Qt${QT_VERSION_MAJOR}::Widgets)


# the executable require file
set(BIN_NEED_FILE
        "chi_tra.traineddata"
        "eng.traineddata")

# move ocr model to executable model folder
foreach(file_name ${BIN_NEED_FILE})
    add_custom_command(TARGET Customize_OCR POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PROJECT_SOURCE_DIR}/bin/${file_name}"
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/model/${file_name}")
endforeach()

