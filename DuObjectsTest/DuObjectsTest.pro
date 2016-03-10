#-------------------------------------------------
#
# Project created by QtCreator 2016-01-12T12:17:50
#
#-------------------------------------------------

QT       += testlib network widgets

QT       -= gui

TARGET = duobjecttest
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app

include(../du_objects.pri)

SOURCES += \
    main.cpp \
    tst_dumusictest.cpp

HEADERS += \
    tst_dumusictest.h

DEFINES += SRCDIR=\\\"$$PWD/..\\\"

INCLUDEPATH += $$PWD/..
