project(Applet)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.8 REQUIRED COMPONENTS Core)

add_executable(${PROJECT_NAME}
#        src/CPUUsageMonitor.cpp
        src/ProcessCPUMonitor.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::Quick
        pdh
)
