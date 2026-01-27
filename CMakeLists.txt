cmake_minimum_required(VERSION 3.10)
project(UmaCalculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCES
    src/main.cpp
    src/calculator.cpp
    src/skill.cpp
    src/stats.cpp
)

add_executable(uma_calculator ${SOURCES})

if(MSVC)
    target_compile_options(uma_calculator PRIVATE /W4)
else()
    target_compile_options(uma_calculator PRIVATE -Wall -Wextra -Wpedantic)
endif()
