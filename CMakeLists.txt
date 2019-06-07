cmake_minimum_required(VERSION 3.7)
project(StateReportable)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set (BRAND_FULLNAME "StateReportable")
set (BRAND_COMPANYNAME "ADDubovik")

set(CMAKE_INSTALL_PREFIX bin)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
include(cmake/projecthelper.cmake)
#DEFINE_DEFAULT_DEFINITIONS()
#SETUP_COMPILER_SETTINGS(OFF)

PRINT_ALL_CMAKE_SETTINGS()

#add_subdirectory(src)

enable_testing()

#add_subdirectory(src/tests)
