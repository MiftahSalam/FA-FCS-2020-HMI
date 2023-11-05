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
    src/infra/http/http_client_wrapper.cpp \
    src/main.cpp \
    src/model/osd/cms/osd_set_position_request.cpp \
    src/model/osd/position_model.cpp \
    src/shared/common/errors/err_base.cpp \
    src/shared/common/errors/err_http.cpp \
    src/shared/common/errors/err_json_parse.cpp \
    src/shared/common/errors/err_object_creation.cpp \
    src/shared/common/errors/err_open_file.cpp \
    src/shared/common/errors/helper_err.cpp \
    src/shared/config/configuration.cpp \
    src/shared/config/osd_cms_config.cpp \
    src/shared/utils/utils.cpp \
    src/usecase/osd/cms/osd_cms.cpp \
    src/usecase/osd/cms/osd_cms_position_data.cpp \
    src/usecase/osd/osd_service.cpp \
    test/shared/config/test_osd_cms_config.cpp \
    test/shared/errors/test_base_error.cpp \
    test/testsuit.cpp \
    test/usecase/osd/cms/test_usecase_osd_cms.cpp

HEADERS += \
    src/di/di.h \
    src/infra/http/http_client_wrapper.h \
    src/model/base_request.h \
    src/model/base_response.h \
    src/model/osd/cms/osd_set_position_request.h \
    src/model/osd/position_model.h \
    src/shared/common/errors/err_base.h \
    src/shared/common/errors/err_http.h \
    src/shared/common/errors/err_json_parse.h \
    src/shared/common/errors/err_object_creation.h \
    src/shared/common/errors/err_open_file.h \
    src/shared/common/errors/helper_err.h \
    src/shared/config/base_config.h \
    src/shared/config/configuration.h \
    src/shared/config/osd_cms_config.h \
    src/shared/utils/utils.h \
    src/usecase/osd/cms/i_osd_cms.h \
    src/usecase/osd/cms/osd_cms.h \
    src/usecase/osd/cms/osd_cms_position_data.h \
    src/usecase/osd/osd_service.h \
    test/shared/config/test_osd_cms_config.h \
    test/shared/errors/test_base_error.h \
    test/testsuit.h \
    test/usecase/osd/cms/test_usecase_osd_cms.h
