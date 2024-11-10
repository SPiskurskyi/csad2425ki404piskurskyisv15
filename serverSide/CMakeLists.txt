cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME server)

project(${PROJECT_NAME})

set(BOARD "arduino:avr:uno")
set(SERIAL_PORT_PATH "/dev/ttyUSB0" CACHE STRING "Serial port path")    

set(SOURCE_FILE ${CMAKE_SOURCE_DIR}/serverSide.ino)

find_program(ARDUINO_CLI arduino-cli)
if(NOT ARDUINO_CLI)
    message(FATAL_ERROR "arduino-cli not found. Please install it and add to PATH.")
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

add_custom_target(build ALL
    COMMAND ${ARDUINO_CLI} compile --fqbn ${BOARD} ${SOURCE_FILE}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Building Arduino sketch..."
)

add_custom_target(upload
    COMMAND ${ARDUINO_CLI} upload -p ${SERIAL_PORT_PATH} --fqbn ${BOARD} ${SOURCE_FILE}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS build
    COMMENT "Uploading Arduino sketch..."
)