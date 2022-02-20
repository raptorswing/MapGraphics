#-------------------------------------------------
#
# Project created by QtCreator 2012-03-03T10:50:47
#
#-------------------------------------------------

QT       += network sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

TARGET = MapGraphics
TEMPLATE = lib

DEFINES += MAPGRAPHICS_LIBRARY

INCLUDEPATH += .

SOURCES += MapGraphicsScene.cpp \
    MapGraphicsObject.cpp \
    MapGraphicsView.cpp \
    guts/PrivateQGraphicsScene.cpp \
    guts/PrivateQGraphicsObject.cpp \
    guts/Conversions.cpp \
    MapTileSource.cpp \
    tileSources/GridTileSource.cpp \
    guts/MapTileGraphicsObject.cpp \
    guts/PrivateQGraphicsView.cpp \
    tileSources/OSMTileSource.cpp \
    guts/MapGraphicsNetwork.cpp \
    tileSources/CompositeTileSource.cpp \
    guts/MapTileLayerListModel.cpp \
    guts/MapTileSourceDelegate.cpp \
    guts/CompositeTileSourceConfigurationWidget.cpp \
    CircleObject.cpp \
    guts/PrivateQGraphicsInfoSource.cpp \
    PolygonObject.cpp \
    Position.cpp \
    LineObject.cpp

HEADERS += MapGraphicsScene.h\
        MapGraphics_global.h \
    MapGraphicsObject.h \
    MapGraphicsView.h \
    guts/PrivateQGraphicsScene.h \
    guts/PrivateQGraphicsObject.h \
    guts/Conversions.h \
    MapTileSource.h \
    tileSources/GridTileSource.h \
    guts/MapTileGraphicsObject.h \
    guts/PrivateQGraphicsView.h \
    tileSources/OSMTileSource.h \
    guts/MapGraphicsNetwork.h \
    tileSources/CompositeTileSource.h \
    guts/MapTileLayerListModel.h \
    guts/MapTileSourceDelegate.h \
    guts/CompositeTileSourceConfigurationWidget.h \
    CircleObject.h \
    guts/PrivateQGraphicsInfoSource.h \
    PolygonObject.h \
    Position.h \
    LineObject.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE4F7F973
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = MapGraphics.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

#unix:!symbian {
#    maemo5 {
#        target.path = /opt/usr/lib
#    } else {
#        target.path = /usr/lib
#    }
#    INSTALLS += target
#}

FORMS += \
    guts/CompositeTileSourceConfigurationWidget.ui

RESOURCES += \
    resources.qrc

# BUILD OUTPUT DIR outside of parent MapGraphics
# MapGraphics_BUILD at the same dir level of MapGraphics
message( 'Building BUILD_OUT_DIR= $${OUT_PWD} ' )


message( 'Building QMAKESPEC= $${QMAKESPEC} ' )
#-------------------------------------------------
# WIN32
#-------------------------------------------------
win32{
    message( 'Building TARGET win32 WINDOWS_TARGET_PLATFORM_VERSION= $${WINDOWS_TARGET_PLATFORM_VERSION}' )
    CONFIG(release, debug|release){
        message( 'Building TARGET win32 release' )
        target.path = $$PWD/../../MapGraphics_BUILD/LIB_BINARY/MSVC2019_64bit-Release
        DESTDIR = $${target.path}
    }
    CONFIG(debug, debug|release){
        message( 'Building TARGET win32 debug' )

    }
}


#-------------------------------------------------
# ANDROID
#-------------------------------------------------
android {
    message('Building android')
    message('Building android ANDROID_TARGET_ARCH $${ANDROID_TARGET_ARCH}')

    contains(ANDROID_TARGET_ARCH,x86_64) {
        message('Building android for x86_64')
        #ANDROID_EXTRA_LIBS =
        # message('Building ANDROID_TARGET_ARCH x86_64  ANDROID_EXTRA_LIBS=$${ANDROID_EXTRA_LIBS}')
        #LIBS += -L$$PWD/ -l
        #INCLUDEPATH += $$PWD/.....
        #DEPENDPATH += $$PWD/......

        #top_builddir=$$shadowed($$PWD)
        #DESTDIR = $$top_builddir/plugins/myplugin
        #DESTDIR = ../TestApp
        target.path = $$PWD/../../MapGraphics_BUILD/LIB_BINARY/Clang_x86_64-Release
        DESTDIR = $${target.path}
        #INSTALLS += target
        message( 'Building android INSTALLS= $${target.path} ' )
    }

    contains(ANDROID_TARGET_ARCH,arm64-v8a) {
        message( 'Building android for arm64-v8a' )
        #ANDROID_EXTRA_LIBS =
        # message('Building ANDROID_TARGET_ARCH x86_64  ANDROID_EXTRA_LIBS=$${ANDROID_EXTRA_LIBS}')
        #LIBS += -L$$PWD/ -l
        #INCLUDEPATH += $$PWD/.....
        #DEPENDPATH += $$PWD/......
        #top_builddir=$$shadowed($$PWD)
        #DESTDIR = $$top_builddir/plugins/myplugin
        #DESTDIR = ../TestApp
        target.path = $$PWD/../../MapGraphics_BUILD/LIB_BINARY/Clang_arm64-v8a-Release
        DESTDIR = $${target.path}
        #INSTALLS += target
        message( 'Building android INSTALLS= $${target.path} ' )
    }

    android: include(C:/TOOLS/ANDROID/android_openssl/openssl.pri)
}

#-------------------------------------------------
# iOs Configuration
#-------------------------------------------------
ios {
    message( 'Building ios : QMAKE_TARGET= $${QMAKE_TARGET} QMAKE_MACOSX_DEPLOYMENT_TARGET= $${QMAKE_MACOSX_DEPLOYMENT_TARGET}' )
     # iOs simulator
    #target.path = $$PWD/../../MapGraphics_BUILD/LIB_BINARY/iOS_Simulator-Release
    #DESTDIR = $${target.path}
    #message( 'Building ios simulator' )

    # iOs phone
    target.path = $$PWD/../../MapGraphics_BUILD/LIB_BINARY/iOS-Release
    DESTDIR = $${target.path}
    message( 'Building ios phone' )

}
