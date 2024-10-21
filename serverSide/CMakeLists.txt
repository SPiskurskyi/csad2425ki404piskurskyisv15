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