#-------------------------------------------------
#
# Project created by QtCreator 2012-03-03T10:50:47
#
#-------------------------------------------------

QT       += network sql

TARGET = MapGraphics
TEMPLATE = lib

DEFINES += MAPGRAPHICS_LIBRARY

SOURCES += MapGraphicsScene.cpp \
    MapGraphicsObject.cpp \
    MapGraphicsView.cpp \
    guts/PrivateQGraphicsScene.cpp \
    guts/PrivateQGraphicsObject.cpp \
    guts/Conversions.cpp \
    MapTileSource.cpp \
    GridTileSource.cpp \
    guts/MapTileGraphicsObject.cpp \
    guts/PrivateQGraphicsView.cpp \
    OSMTileSource.cpp \
    guts/MapGraphicsNetwork.cpp \
    CompositeTileSource.cpp \
    guts/SQLiteKeyValueStore.cpp \
    TileSourceConfigurationWidget.cpp \
    MapTileLayerListModel.cpp

HEADERS += MapGraphicsScene.h\
        MapGraphics_global.h \
    MapGraphicsObject.h \
    MapGraphicsView.h \
    guts/PrivateQGraphicsScene.h \
    guts/PrivateQGraphicsObject.h \
    guts/Conversions.h \
    MapTileSource.h \
    GridTileSource.h \
    guts/MapTileGraphicsObject.h \
    guts/PrivateQGraphicsView.h \
    OSMTileSource.h \
    guts/MapGraphicsNetwork.h \
    CompositeTileSource.h \
    guts/SQLiteKeyValueStore.h \
    TileSourceConfigurationWidget.h \
    MapTileLayerListModel.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE4F7F973
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = MapGraphics.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

FORMS += \
    TileSourceConfigurationWidget.ui
