cmake_minimum_required(VERSION 3.16)

project(CrossProcess VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.8 REQUIRED COMPONENTS Core)

qt_standard_project_setup(REQUIRES 6.8)

add_library(${PROJECT_NAME}
        STATIC
        src/CrossProcess.cpp
        src/CrossProcess.h
        src/ProcessInfo.cpp
        src/ProcessInfo.h
)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        src
)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        Qt6::Core
)
