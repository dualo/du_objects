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
    tst_dusampletest.cpp \
    tst_dusoundtest.cpp \
    tst_dumusictest.cpp \
    tst_dueffectstest.cpp \
    tst_dupresettest.cpp

HEADERS += \
    tst_dusampletest.h \
    tst_dusoundtest.h \
    tst_dumusictest.h \
    tst_dueffectstest.h \
    tst_dupresettest.h

DEFINES += SRCDIR=\\\"$$PWD/..\\\"

INCLUDEPATH += $$PWD/..
