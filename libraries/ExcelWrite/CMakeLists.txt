cmake_minimum_required(VERSION 3.5)

project(ExcelWrite VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)


# Excel - Option
# -----------------------------------------------------------------------------------------------
if(NOT DEFINED ${QXLSX_PARENTPATH})
	set(QXLSX_PARENTPATH ${CMAKE_CURRENT_SOURCE_DIR}/..)
endif(NOT DEFINED ${QXLSX_PARENTPATH}) 
	
if(NOT DEFINED ${QXLSX_HEADERPATH})	
	set(QXLSX_HEADERPATH ${CMAKE_CURRENT_SOURCE_DIR}/../QXlsx/header)
endif(NOT DEFINED ${QXLSX_HEADERPATH})		

if(NOT DEFINED ${QXLSX_SOURCEPATH})
	set(QXLSX_SOURCEPATH ${CMAKE_CURRENT_SOURCE_DIR}/../QXlsx/source)
endif(NOT DEFINED ${QXLSX_SOURCEPATH})	

message("Current Path of QXlsx")
message(${QXLSX_PARENTPATH})
message(${QXLSX_HEADERPATH})
message(${QXLSX_SOURCEPATH})

include_directories(${QXLSX_HEADERPATH})

file(GLOB QXLSX_CPP "${QXLSX_SOURCEPATH}/*.cpp")
file(GLOB QXLSX_H "${QXLSX_HEADERPATH}/*.h")

set(QXlse_SRC_FILES ${QXLSX_CPP})
list(APPEND QXlse_SRC_FILES ${QXLSX_H})
 

set(PROJECT_SOURCES

	"./testing/main.cpp"
	"ExcelWrite.h"
	"ExcelWrite.cpp"
        
)


# list(APPEND PROJECT_SOURCES ${QXlse_SRC_FILES})

add_executable(ExcelWrite ${PROJECT_SOURCES} ${QXlse_SRC_FILES})

target_link_libraries(ExcelWrite PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${DEPS} Qt${QT_VERSION_MAJOR}::GuiPrivate)
