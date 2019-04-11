#-------------------------------------------------
#
# Project created by QtCreator 2018-03-04T17:27:06
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RC_FILE = icon.rc
TARGET = SerialDebug
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    about.cpp \
    version.cpp \
    setting.cpp \
    ascii.cpp \
    log.cpp

HEADERS  += mainwindow.h \
    about.h \
    version.h \
    setting.h \
    ascii.h \
    log.h

FORMS    += mainwindow.ui \
    about.ui \
    version.ui \
    setting.ui \
    ascii.ui \
    log.ui

RESOURCES += \
    imageResource.qrc
