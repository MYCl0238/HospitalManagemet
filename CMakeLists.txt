cmake_minimum_required(VERSION 3.28)
project(HospitalManagement)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif ()

include_directories(include)

file(GLOB LIB_SOURCES "include/*.cpp")
file(GLOB LIB_HEADERS "include/*.h")

add_library(HospitalManagementLib ${LIB_SOURCES} ${LIB_HEADERS})

add_executable(HospitalManagement main.cpp)

set_target_properties(HospitalManagementLib PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(HospitalManagement HospitalManagementLib)
