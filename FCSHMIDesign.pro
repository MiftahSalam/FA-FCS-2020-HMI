#-------------------------------------------------
#
# Project created by QtCreator 2020-01-20T10:28:26
#
#-------------------------------------------------

QT       += core gui network

win32: {
    message("Build for windows")
    QT += network
    include(MyQtRedis/QtRedis.pri)
} else:linux {
    message("Build for linux")
    include(qredis/qredis.pri)
    CONFIG += c++11
}
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GunControllerLPDP
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    framegun.cpp \
    frameosd.cpp \
    framewap.cpp \
    frametda.cpp \
    track.cpp \
    tda_global.cpp \
    rceventhandler.cpp \
    redisconnectionnotify.cpp

HEADERS  += mainwindow.h \
    framegun.h \
    frameosd.h \
    framewap.h \
    global.h \
    frametda.h \
    track.h \
    tda_global.h \
    rceventhandler.h \
    redisconnectionnotify.h

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
