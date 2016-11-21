#-------------------------------------------------
#
# Project created by QtCreator 2016-01-20T11:00:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = contours
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    canvaswidget.cpp

HEADERS  += mainwindow.h \
    canvaswidget.h

FORMS    += mainwindow.ui


CONFIG += link_pkgconfig
PKGCONFIG += opencv
