#-------------------------------------------------
#
# Project created by QtCreator 2020-01-20T10:28:26
#
#-------------------------------------------------

QT       += core gui

include($$PWD/qredis/qredis.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FCSHMIDesign
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    framegun.cpp \
    frameosd.cpp \
    framewap.cpp

HEADERS  += mainwindow.h \
    framegun.h \
    frameosd.h \
    framewap.h

FORMS    += mainwindow.ui \
    framegun.ui \
    frameosd.ui \
    framewap.ui

unix:!macx: LIBS += -lhiredis
