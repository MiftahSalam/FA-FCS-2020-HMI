#-------------------------------------------------
#
# Project created by QtCreator 2020-01-20T10:28:26
#
#-------------------------------------------------

QT       += core gui network testlib

#include($$PWD/qredis/qredis.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fa_fcs_hmi
TEMPLATE = app

SOURCES += \
    src/model/osd/cms/osd_set_position_reqeust.cpp \
    src/main.cpp \
    src/usecase/osd/cms/osd_cms.cpp \
    src/usecase/osd/osd_service.cpp \
    test/testsuit.cpp \
    test/usecase/osd/cms/test_usecase_osd_cms.cpp

HEADERS += \
    src/model/osd/cms/osd_set_position_reqeust.h \
    src/usecase/osd/cms/osd_cms.h \
    src/usecase/osd/osd_service.h \
    test/testsuit.h \
    test/usecase/osd/cms/test_usecase_osd_cms.h
