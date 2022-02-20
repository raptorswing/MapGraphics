#-------------------------------------------------
#
# Project created by QtCreator 2012-03-03T10:51:15
#
#-------------------------------------------------

QT       += core gui network sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

TARGET = TestApp
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp

HEADERS  += MainWindow.h

FORMS    += MainWindow.ui

#Linkage for MapGraphics shared library
#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../MapGraphics/release/ -lMapGraphics
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../MapGraphics/debug/ -lMapGraphics
#else:unix:!symbian: LIBS += -L$$OUT_PWD/../MapGraphics/ -lMapGraphics

INCLUDEPATH += $$PWD/../MapGraphics
DEPENDPATH += $$PWD/../MapGraphics

# BUILD OUTPUT DIR outside of parent MapGraphics
# MapGraphics_BUILD at the same dir level of MapGraphics
message( 'Building BUILD_OUT_DIR= $${OUT_PWD} ' )

message( 'Building QMAKESPEC= $${QMAKESPEC} ' )

#-------------------------------------------------
# WIN32 MSVC
#-------------------------------------------------
win32{
    message( 'Building TARGET win32' )
    CONFIG(release, debug|release){
        message( 'Building TARGET win32 release' )
        LIBS += -L$$PWD/../../MapGraphics_BUILD/LIB_BINARY/MSVC2019_64bit-Release/ -lMapGraphics
        INCLUDEPATH += $$PWD/../../MapGraphics_BUILD/LIB_BINARY/MSVC2019_64bit-Release
        DEPENDPATH += $$PWD/../../MapGraphics_BUILD/LIB_BINARY/MSVC2019_64bit-Release
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
    message('Building TARGET ANDROID_TARGET_ARCH $${ANDROID_TARGET_ARCH}')

    contains(ANDROID_TARGET_ARCH,x86_64) {
        message('Building android for x86_64')
        ANDROID_EXTRA_LIBS = $$PWD/../../MapGraphics_BUILD/LIB_BINARY/Clang_x86_64-Release/libMapGraphics_x86_64.so
        LIBS += -L$$PWD/../../MapGraphics_BUILD/LIB_BINARY/Clang_x86_64-Release/ -lMapGraphics_x86_64
        INCLUDEPATH += $$PWD/../../MapGraphics_BUILD/LIB_BINARY/Clang_x86_64-Release
        DEPENDPATH += $$PWD/../../MapGraphics_BUILD/LIB_BINARY/Clang_x86_64-Release
        message('Building android for x86_64  ANDROID_EXTRA_LIBS=$${ANDROID_EXTRA_LIBS}')
    }

    contains(ANDROID_TARGET_ARCH,arm64-v8a) {
        message( 'Building android for arm64-v8a' )
        ANDROID_EXTRA_LIBS = $$PWD/../../MapGraphics_BUILD/LIB_BINARY/Clang_arm64-v8a-Release/libMapGraphics_arm64-v8a.so
        LIBS += -L$$PWD/../../MapGraphics_BUILD/LIB_BINARY/Clang_arm64-v8a-Release/ -lMapGraphics_arm64-v8a
        INCLUDEPATH += $$PWD/../../MapGraphics_BUILD/LIB_BINARY/Clang_arm64-v8a-Release
        DEPENDPATH += $$PWD/../../MapGraphics_BUILD/LIB_BINARY/Clang_arm64-v8a-Release
        message('Building android for x86_64  ANDROID_EXTRA_LIBS=$${ANDROID_EXTRA_LIBS}')
    }
    android: include(C:/TOOLS/ANDROID/android_openssl/openssl.pri)

}

#-------------------------------------------------
# iOs Configuration
#-------------------------------------------------
ios{
    # message( 'Building ios : QMAKE_MACOSX_DEPLOYMENT_TARGET= $${QMAKE_MACOSX_DEPLOYMENT_TARGET}' )
    # iOs simulator
    #LIBS += -L$$PWD/../../MapGraphics_BUILD/LIB_BINARY/iOS_Simulator-Release/ -lMapGraphics
    #INCLUDEPATH += $$PWD/../../MapGraphics_BUILD/LIB_BINARY/iOS_Simulator-Release
    #DEPENDPATH += $$PWD/../../MapGraphics_BUILD/LIB_BINARY/iOS_Simulator-Release
    #message('Building ios simulator')

    # iOs phone
    LIBS += -L$$PWD/../../MapGraphics_BUILD/LIB_BINARY/iOS-Release/ -lMapGraphics
    INCLUDEPATH += $$PWD/../../MapGraphics_BUILD/LIB_BINARY/iOS-Release
    DEPENDPATH += $$PWD/../../MapGraphics_BUILD/LIB_BINARY/iOS-Release
    message('Building ios')

}


#unix:!symbian
#{
    #LIBS += -L$$OUT_PWD/../MapGraphics/ -lMapGraphics
#}

message('Building using LIBS= $${LIBS} ')
