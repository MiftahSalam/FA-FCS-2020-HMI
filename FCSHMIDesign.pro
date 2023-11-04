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
    src/di/di.cpp \
    src/model/osd/cms/osd_set_position_reqeust.cpp \
    src/main.cpp \
    src/shared/common/errors/base_error.cpp \
    src/shared/common/errors/err_open_file.cpp \
    src/shared/config/configuration.cpp \
    src/shared/config/osd_cms_config.cpp \
    src/usecase/osd/cms/osd_cms.cpp \
    src/usecase/osd/osd_service.cpp \
    test/shared/config/test_osd_cms_config.cpp \
    test/shared/errors/test_base_error.cpp \
    test/testsuit.cpp \
    test/usecase/osd/cms/test_usecase_osd_cms.cpp

HEADERS += \
    src/di/di.h \
    src/model/osd/cms/osd_set_position_reqeust.h \
    src/shared/common/errors/base_error.h \
    src/shared/common/errors/err_open_file.h \
    src/shared/config/base_config.h \
    src/shared/config/configuration.h \
    src/shared/config/osd_cms_config.h \
    src/usecase/osd/cms/osd_cms.h \
    src/usecase/osd/osd_service.h \
    test/shared/config/test_osd_cms_config.h \
    test/shared/errors/test_base_error.h \
    test/testsuit.h \
    test/usecase/osd/cms/test_usecase_osd_cms.h
