# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.13)

project("displite_lib"
VERSION 0.3
DESCRIPTION "cpp library for displite"
HOMEPAGE_URL "https://github.com/displite/displite_lib"
)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    add_subdirectory(linux)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    message(FATAL_ERROR "Windows is currently not supported by this version of lib")
else()
    message(FATAL_ERROR "OS not supported")
endif()
