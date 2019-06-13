cmake_minimum_required(VERSION 3.7)
project(StateReportable)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set (BRAND_FULLNAME "StateReportable")
set (BRAND_COMPANYNAME "ADDubovik")

if (WIN32)
	cmake_policy(SET CMP0074 NEW)
endif()

set(CMAKE_INSTALL_PREFIX bin)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
include(cmake/projecthelper.cmake)
#DEFINE_DEFAULT_DEFINITIONS()
#SETUP_COMPILER_SETTINGS(OFF)

PRINT_ALL_CMAKE_SETTINGS()

add_subdirectory(src/StateReportable)

option(BUILD_TESTS "Whether or not to build tests, gtest submodule is required" ON)

if(WIN32)
    set(ADDITIONAL_LIBS )
else()
    set(ADDITIONAL_LIBS -std=c++17 -lstdc++fs -lpthread)
endif()


if (BUILD_TESTS)
	enable_testing()
	add_subdirectory(src/tests)
endif()
