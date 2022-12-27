cmake_minimum_required(VERSION 3.0)

project(MapGraphics)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Widgets Network REQUIRED)

qt5_add_resources(PROJECT_SOURCES resources.qrc)

add_library(MapGraphics SHARED 
                        tileSources/CompositeTileSource.cpp
                        tileSources/GridTileSource.cpp
                        tileSources/OSMTileSource.cpp
                        guts/PrivateQGraphicsObject.cpp
                        guts/PrivateQGraphicsInfoSource.cpp
                        guts/PrivateQGraphicsView.cpp
                        guts/PrivateQGraphicsScene.cpp
                        guts/MapGraphicsNetwork.cpp
                        guts/MapTileLayerListModel.cpp
                        guts/CompositeTileSourceConfigurationWidget.cpp
                        guts/MapTileSourceDelegate.cpp
                        guts/Conversions.cpp
                        guts/MapTileGraphicsObject.cpp
                        CircleObject.cpp
                        MapGraphicsView.cpp
                        MapTileSource.cpp
                        PolygonObject.cpp
                        MapGraphicsScene.cpp
                        Position.cpp
                        LineObject.cpp
                        MapGraphicsObject.cpp )

target_link_libraries( MapGraphics Qt5::Widgets Qt5::Network )

set_target_properties(MapGraphics PROPERTIES
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
    SOVERSION ${VERSION_MAJOR} )
