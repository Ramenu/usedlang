cmake_minimum_required(VERSION 3.10)

set(CMAKE_BUILD_TYPE "Release")

# Set compiler
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# Executable directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ..)

# Name of the project
set(PROGRAM_NAME usedlang)
project(${PROGRAM_NAME})

# Set the C++ standard (C++20 by default)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Add the executable name, and the source files
add_executable(${PROGRAM_NAME}
./src/main.cpp
./src/error.cpp
./src/file.cpp)


# Compiler flags
target_compile_options(${PROGRAM_NAME} PRIVATE 
-fdiagnostics-color
-Wall
-Werror
-Wpedantic
-Wextra
-ffunction-sections
-fdata-sections
-fstrict-enums
-fsanitize=leak
-Wctor-dtor-privacy
-Wnon-virtual-dtor
-Wmismatched-tags
-Wextra-semi
-Wsuggest-final-types
-Wsuggest-final-methods
-Wsuggest-override
-Wuseless-cast
-Wdouble-promotion
-Wimplicit-fallthrough
-Wmissing-include-dirs
-Wformat=2
-fPIC
-Wduplicated-branches
-Wduplicated-cond
-Wshadow
-Wfloat-equal
-Wcast-qual
-Wcast-align
-Wnull-dereference
-Winvalid-pch
-Winline
-Wunsafe-loop-optimizations
-Wlogical-op
-Wdisabled-optimization
-Wvirtual-inheritance
)


# Options for release build
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Running release build")
    target_compile_options(${PROGRAM_NAME} PRIVATE 
    -DNDEBUG
    -O3
    -flto
    -march=native)
    target_link_options(${PROGRAM_NAME} PUBLIC -flto -s)

endif (CMAKE_BUILD_TYPE STREQUAL "Release")

# Linker flags (optional)
# target_link_options(___ PUBLIC ___)

# Set timer for build
set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")
set_property(TARGET ${PROGRAM_NAME} PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")

# Include directories
target_include_directories(${PROGRAM_NAME} PUBLIC 
"./include")
target_link_directories(${PROGRAM_NAME} PUBLIC "./lib")

# Libraries to link with (optional)
target_link_libraries(${PROGRAM_NAME} PUBLIC fmt simdjson)