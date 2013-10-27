#-------------------------------------------------
#
# Project created by QtCreator 2013-10-26T21:01:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CompGraphicsL2
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp\
        widget.cpp \
    MMatrix.cpp \
    mpolygon.cpp

HEADERS  += widget.h \
    MMatrix.h \
    mpolygon.h

FORMS    += widget.ui
