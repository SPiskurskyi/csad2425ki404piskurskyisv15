cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME client)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
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

target_link_libraries(${PROJECT_NAME} ${SERIALPORT_LIB})

#-------------------------------------------
# Doxygen configuration
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

#-------------------------------------------
# Test configuration
find_package(GTest REQUIRED)

if (GTest_FOUND)
    add_custom_target(tests
        COMMENT "Building and running tests"
    )

    add_custom_command(TARGET tests PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Building test executable"
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target game_tests
    )

    set(TEST_SOURCES 
        test/test_main.cpp
        test/test_clientCore.cpp
        test/test_cfgFile.cpp
        # test/test_serialUtils.cpp TBD
        src/serialUtils.cpp
        src/clientCore.cpp
        src/cfgFile.cpp
    )

    add_executable(game_tests EXCLUDE_FROM_ALL ${TEST_SOURCES})

    target_link_libraries(game_tests 
        PRIVATE
        gmock
        GTest::GTest
        GTest::Main 
        ${SERIALPORT_LIB}
    )

    target_compile_definitions(game_tests PRIVATE SERIAL_PORT_PATH=\"${SERIAL_PORT_PATH}\")

    target_include_directories(game_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(game_tests PRIVATE --coverage)
        target_link_options(game_tests PRIVATE --coverage)
    endif ()

    enable_testing()
    add_test(NAME RunTests COMMAND game_tests)
else ()
    message("GoogleTest needs to be installed to run the tests")
endif ()
