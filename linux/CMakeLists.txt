add_library(displite STATIC "displite.cpp" "hidraw.cpp" "exceptions.cpp")
target_include_directories(displite PRIVATE "${CMAKE_CURRENT_LIST_DIR}/includes")
target_include_directories(displite PUBLIC "${CMAKE_CURRENT_LIST_DIR}/inc")
target_link_libraries(displite PUBLIC "systemd")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("displite lib in debug mode")
    target_compile_definitions(displite PRIVATE DEBUG)
endif()