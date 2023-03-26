# Basic Setting
cmake_minimum_required(VERSION 3.5)

project(Customize_OCR VERSION 1.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(MSYS ON)
set(CONAN OFF)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if (MSYS)
    set(MSYS2_ROOT_PATH "C:/Users/idk/opt/msys64")
    set(CMAKE_PREFIX_PATH "${MSYS2_ROOT_PATH}/mingw64/lib/cmake")
    link_directories("${MSYS2_ROOT_PATH}/mingw64/lib")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
elseif (CONAN)
        # using conan package manager and run conan setup
        include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
        conan_basic_setup()
endif()

include_directories(libraries)

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

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
        libraries/Config/Files_Path/Files_Path.cpp
        libraries/Config/Files_Path/Files_Path.h
        libraries/Config/Global_Config/Global_Config.cpp
        libraries/Config/Global_Config/Global_Config.h)

set(Lib_Utility
        libraries/Utility/Files_Manager/Files_Manager.cpp)


set(Lib_OCR
        libraries/OCR/ImageOCR.cpp libraries/OCR/ImageOCR.h)

set(Lib_ImageFormat
        libraries/Utility/ImageFormat/ImageFormat.cpp
        libraries/Utility/ImageFormat/ImageFormat.h)

set(Lib_XlntControl
        libraries/XlntControl/XlntControl.h
        libraries/XlntControl/XlntControl.cpp
        )


set(SRC_Struct
        src/struct/OCR_Data/OCR_Data.cpp
        src/struct/OCR_Data/OCR_Data.h
        )

set(PROJECT_SOURCES
        src/main.cpp
        ${SRC_Struct}
        ${Lib_Config}
        ${Qt_Widgets}
        ${Lib_Utility}
        ${Lib_OCR}
        ${Lib_ImageFormat}
        ${Lib_XlntControl}
         )

add_executable(Customize_OCR ${PROJECT_SOURCES})

if (MSYS)
    target_link_libraries(Customize_OCR PUBLIC  Qt${QT_VERSION_MAJOR}::Widgets ${OpenCV_LIBS}
            -llibleptonica -llibleptonica -llibtesseract -lxlnt)
else()
    target_link_libraries(Customize_OCR PRIVATE ${CONAN_LIBS} Qt${QT_VERSION_MAJOR}::Widgets xlnt::xlnt)
endif()


# the executable require file
set(BIN_NEED_FILE
        "model/chi_tra.traineddata"
        "model/eng.traineddata"
        )



# move ocr model to executable model folder
foreach(file_name ${BIN_NEED_FILE})
    add_custom_command(TARGET Customize_OCR POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PROJECT_SOURCE_DIR}/bin/${file_name}"
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${file_name}")
endforeach()

message(${CMAKE_BINARY_DIR})

##foreach(file_name ${_IMPORT_CHECK_FILES_FOR_xlnt::xlnt})
#    # copy xlnt DLL into xlnt.test directory
#    add_custom_command(TARGET Customize_OCR POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E copy_if_different
#            "${_IMPORT_PREFIX}/lib/xlnt.lib"
#            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/a.dll")
##endforeach()