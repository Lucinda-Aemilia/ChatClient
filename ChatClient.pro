#-------------------------------------------------
#
# Project created by QtCreator 2016-09-03T21:29:10
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChatClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    signindialog.cpp \
    chatworker.cpp \
    talkwidget.cpp

HEADERS  += mainwindow.h \
    signindialog.h \
    chatworker.h \
    talkwidget.h

FORMS    += mainwindow.ui \
    signindialog.ui \
    talkwidget.ui
