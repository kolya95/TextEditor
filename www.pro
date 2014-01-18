#-------------------------------------------------
#
# Project created by QtCreator 2013-11-14T13:01:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = www
TEMPLATE = app
INCLUDEPATH += /usr/include/python3.3m
LIBS += -L/usr/lib64 -lpython3.3m
DEFINES += QT_NO_KEYWORDS
SOURCES += main.cpp\
        mainwindow.cpp \
    runner.cpp \
    highlighter.cpp \
    kumirinterface.cpp

HEADERS  += mainwindow.h \
    runner.h \
    highlighter.h \
    actorinterface.h \
    kumirinterface.h \
    plugininterface.h

FORMS    += mainwindow.ui
