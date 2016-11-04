#-------------------------------------------------
#
# Project created by QtCreator 2016-10-31T19:30:41
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = player
TEMPLATE = app


SOURCES += main.cpp\
        myplayer.cpp

HEADERS  += myplayer.h

FORMS    += myplayer.ui

RESOURCES += \
    image.qrc
