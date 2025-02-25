cmake_minimum_required(VERSION 3.16)

project(RMM VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.8 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appRMM
        src/cpp/MetricsThread.cpp
        src/cpp/MetricsThread.h
        src/cpp/ProcessMetrics.cpp
        src/cpp/ProcessMetrics.h
        src/cpp/main.cpp
)

qt_add_qml_module(appRMM
    URI RMM
    VERSION 1.0
    QML_FILES
        src/qml/Main.qml
        SOURCES src/cpp/systemmonitor.cpp
        SOURCES src/cpp/systemmonitor.h
        SOURCES src/cpp/ScriptView.cpp
        SOURCES src/cpp/ScriptView.h
)

target_include_directories(appRMM
    PRIVATE
        src/cpp
)
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appRMM PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appRMM
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

add_subdirectory(CrossProcess)
add_subdirectory(Applet)

target_link_libraries(appRMM
    PRIVATE Qt6::Quick
    PRIVATE CrossProcess
)

include(GNUInstallDirs)
install(TARGETS appRMM
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
