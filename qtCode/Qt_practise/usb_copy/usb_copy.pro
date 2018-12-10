#-------------------------------------------------
#
# Project created by QtCreator 2018-11-09T17:25:11
#
#-------------------------------------------------

QT       += core gui charts network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = usb_copy
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    usb_main.cpp \
    usb_copy.cpp \
    searchthread.cpp \
    copythread.cpp \
    generatelogfile.cpp \
    ftpthread.cpp \
    ftp_traversing.cpp \
    ftpconfig.cpp

HEADERS += \
        mainwindow.h \
    config.h \
    usb_copy.h \
    usb_main.h \
    searchthread.h \
    copythread.h \
    ftpthread.h \
    ftp_traversing.h \
    ftpconfig.h

FORMS += \
        mainwindow.ui \
    ftpconfig.ui

LIBS += -lpthread

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
