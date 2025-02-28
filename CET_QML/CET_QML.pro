QT       += core gui quick qml quickwidgets sql multimedia charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
CONFIG += qmltypes
QML_IMPORT_NAME = MYQML    # QML 模块名
QML_IMPORT_MAJOR_VERSION = 1

android {
    ANDROID_NDK_ROOT = E:/Android_SDK/ndk/26.1.10909125
    ANDROID_NDK_VERSION = 26.1.10909125
    ANDROID_ABIS = armeabi-v7a arm64-v8a
}

SOURCES += \
    ac_signal.cpp \
    datastorage.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ac_signal.h \
    datastorage.h \
    mainwindow.h
    ANDROID_ABIS = arm64-v8a
FORMS +=
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Welcome.qml \
    learning1.qml \
    main.qml \
    primary.qml \
    primary_interface.qml \
    stage_five.qml \
    stage_four.qml \
    stage_one.qml \
    stage_six.qml \
    stage_three.qml \
    stage_two.qml \
    word_show_interface.qml

RESOURCES += \
    res.qrc
QML_IMPORT_PATH = $$PWD
QML_DESIGNER_IMPORT_PATH = $$PWD
