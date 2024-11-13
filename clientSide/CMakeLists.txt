cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME client)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SERIAL_PORT_PATH "/dev/ttyUSB0" CACHE STRING "Serial port path")

set(SOURCES 
    src/clientSide.cpp
    src/serialUtils.cpp
    src/clientCore.cpp
    src/cfgFile.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

include_directories(${CMAKE_SOURCE_DIR}/include)

target_compile_definitions(${PROJECT_NAME} PRIVATE SERIAL_PORT_PATH=\"${SERIAL_PORT_PATH}\")

find_library(SERIALPORT_LIB NAMES serialport)
if (NOT SERIALPORT_LIB)
    message(FATAL_ERROR "libserialport not found")
endif()

find_package(Doxygen REQUIRED)

if (DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
else ()
    message("Doxygen needs to be installed to generate the documentation")
endif ()

target_link_libraries(${PROJECT_NAME} ${SERIALPORT_LIB})
