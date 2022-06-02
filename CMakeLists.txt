cmake_minimum_required(VERSION 3.10)

# Set the build to debug mode
set(CMAKE_BUILD_TYPE "Release")

# Set compiler
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Executable directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ..)

# Name of the project
set(PROGRAM_NAME usedlang)
project(${PROGRAM_NAME})

# Set the C standard (C11 by default)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Set the C++ standard (C++20 by default)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Add the executable name, and the source files
add_executable(${PROGRAM_NAME}
./src/main.cpp
./src/file.cpp
./src/error.cpp)


# Compiler flags
target_compile_options(${PROGRAM_NAME} PRIVATE 
-fdiagnostics-color
-fPIC
-Werror
-Wpedantic
-Wall
-Wextra
-Walloca
-Wbad-function-cast
-Wcast-align
-Wcast-qual
-Wconversion
-Wctad-maybe-unsupported # In case template argument deduction does not work on a fn
-Wdate-time
-Wdeprecated
-Wdouble-promotion
-Wdtor-name
-Wduplicate-decl-specifier
-Wembedded-directive
-Wempty-translation-unit
-Wenum-conversion
-Wflexible-array-extensions
-Wfloat-equal
-Wfloat-conversion
-Wglobal-constructors
-Wheader-hygiene
-Wimplicit-fallthrough
-Winconsistent-missing-destructor-override
-Wkeyword-macro
-Wloop-analysis
-Wmicrosoft
-Wnarrowing
-Wnon-virtual-dtor
-Wold-style-cast
-Wover-aligned
-Woverloaded-virtual
-Wpadded
-Wpointer-arith
-Wpoison-system-directories
-Wredundant-move
-Wretained-language-linkage
-Wself-assign-overloaded
-Wself-move
-Wshadow-all
-Wsign-compare
-Wsign-conversion
-Wsometimes-uninitialized
-Wstring-concatenation
-Wstring-conversion
-Wsuggest-destructor-override
-Wsuggest-override
-Wsuper-class-method-mismatch
-Wthread-safety
-Wundefined-reinterpret-cast
-Wundefined-internal-type
-Wuninitialized
-Wunreachable-code
-Wvector-conversion
-Wzero-length-array

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
endif()

# Include directories
target_include_directories(${PROGRAM_NAME} PUBLIC "./include")

# Libraries to link with (optional)
target_link_libraries(${PROGRAM_NAME} PUBLIC fmt simdjson) 