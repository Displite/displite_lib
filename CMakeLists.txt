# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.13)

project ("displite_lib")

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    add_subdirectory(linux)
endif()