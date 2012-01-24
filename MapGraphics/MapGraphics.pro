#-------------------------------------------------
#
# Project created by QtCreator 2011-09-30T13:47:16
#
#-------------------------------------------------

QT       += network gui core

TARGET = MapGraphics
TEMPLATE = lib

DEFINES += MAPGRAPHICS_LIBRARY

SOURCES += guts/MGNetworkManager.cpp \
    guts/MapGraphicsScene.cpp \
    guts/PrivateQGraphicsView.cpp \
    MapGraphicsView.cpp \
    guts/MapTileGraphicsItem.cpp \
    guts/MapInfoManager.cpp \
    guts/MapTileSource.cpp \
    guts/MapQuestTileSource.cpp \
    MapGraphicsItem.cpp \
    CompassWidget.cpp \
    guts/PrivateQGraphicsItem.cpp \
    guts/PrivateQGraphicsItemParent.cpp \
    ZoomWidget.cpp

HEADERS += guts/MGNetworkManager.h\
        guts/MapGraphics_global.h \
    guts/MapGraphicsScene.h \
    guts/PrivateQGraphicsView.h \
    MapGraphicsView.h \
    MapGraphicsItem.h \
    guts/MapTileGraphicsItem.h \
    guts/MapInfoManager.h \
    guts/MapTileSource.h \
    guts/MapQuestTileSource.h \
    CompassWidget.h \
    guts/PrivateQGraphicsItem.h \
    guts/PrivateQGraphicsItemParent.h \
    ZoomWidget.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE47F7904
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

FORMS += MapGraphicsView.ui



































