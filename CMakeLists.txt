cmake_minimum_required(VERSION 3.12)
project(MyCppProject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify the source files
set(SOURCES
    src/test.cpp
)

include_directories(include)

# Add warnings for GCC/Clang

add_executable(MyCppProject ${SOURCES})

target_compile_options(MyCppProject PRIVATE -O0 -g -Wall -Wextra -Wpedantic -march=native) # Use for debug: -O0 -g -Wall -Wextra -Wpedantic