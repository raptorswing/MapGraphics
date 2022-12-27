cmake_minimum_required(VERSION 3.0)

project(TestApp)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

include_directories( ../MapGraphics )

add_executable(TestApp main.cpp MainWindow.cpp MainWindow.h MainWindow.ui)

target_link_libraries( TestApp Qt5::Widgets MapGraphics )

