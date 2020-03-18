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
    framewap.cpp \
    frametda.cpp \
    track.cpp \
    tda_global.cpp \
    rceventhandler.cpp

HEADERS  += mainwindow.h \
    framegun.h \
    frameosd.h \
    framewap.h \
    global.h \
    frametda.h \
    track.h \
    tda_global.h \
    rceventhandler.h

FORMS    += mainwindow.ui \
    framegun.ui \
    frameosd.ui \
    framewap.ui \
    frametda.ui

RESOURCES += \
    hmi_fa_fcs.qrc

DISTFILES += \
    HMI_Syle.css \
    logofcs.png
