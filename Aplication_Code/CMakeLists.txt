cmake_minimum_required(VERSION 3.19)
project(APLICACION_COMUNICACION LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets SerialPort)

qt_standard_project_setup()

qt_add_executable(APLICACION_COMUNICACION
    WIN32
    MACOSX_BUNDLE

    main.cpp

    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    canmanager.cpp
    canmanager.h
    canmanager.h canmanager.cpp
    canmanager.cpp
    canmanager.cpp
    canmanager.cpp
    robotcontroller.h robotcontroller.cpp
    inclinacionwidget.h inclinacionwidget.cpp
)

target_link_libraries(APLICACION_COMUNICACION
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::SerialPort
)

install(TARGETS APLICACION_COMUNICACION
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET APLICACION_COMUNICACION
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
