#-------------------------------------------------
#
# Project created by QtCreator 2013-11-14T13:01:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = www
TEMPLATE = app
QMAKE_CXXFLAGS += $$system(python3-config --cflags)
LIBS += $$system(python3-config --libs)
DEFINES += QT_NO_KEYWORDS
SOURCES += main.cpp\
        mainwindow.cpp \
    runner.cpp \
    highlighter.cpp \
    kumirinterface.cpp \
    mytextedit.cpp

HEADERS  += mainwindow.h \
    runner.h \
    highlighter.h \
    actorinterface.h \
    kumirinterface.h \
    plugininterface.h \
    mytextedit.h

FORMS    += mainwindow.ui
