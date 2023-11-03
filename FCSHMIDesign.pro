#-------------------------------------------------
#
# Project created by QtCreator 2020-01-20T10:28:26
#
#-------------------------------------------------

QT       += core gui network

include($$PWD/qredis/qredis.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fa_fcs_hmi
TEMPLATE = app

SOURCES += \
    osdservice.cpp \
    src/main.cpp

HEADERS += \
    osdservice.h
