#-------------------------------------------------
#
# Project created by QtCreator 2020-01-20T10:28:26
#
#-------------------------------------------------

QT       += core gui network testlib

#include($$PWD/qredis/qredis.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#DEFINES += USE_AMQP

TARGET = fa_fcs_hmi
TEMPLATE = app

SOURCES += \
    src/di/di.cpp \
    src/infra/http/http_client_wrapper.cpp \
    src/infra/messaging/tcp/tcp_messaging_wrapper.cpp \
    src/main.cpp \
    src/model/osd/cms/osd_input_mode_request.cpp \
    src/model/osd/cms/osd_set_position_request.cpp \
    src/model/osd/input_mode_model.cpp \
    src/model/osd/position_model.cpp \
    src/shared/common/errors/err_amqp.cpp \
    src/shared/common/errors/err_base.cpp \
    src/shared/common/errors/err_http.cpp \
    src/shared/common/errors/err_json_parse.cpp \
    src/shared/common/errors/err_messaging.cpp \
    src/shared/common/errors/err_object_creation.cpp \
    src/shared/common/errors/err_open_file.cpp \
    src/shared/common/errors/err_osd_data.cpp \
    src/shared/common/errors/helper_err.cpp \
    src/shared/config/amqp_config.cpp \
    src/shared/config/configuration.cpp \
    src/shared/config/messaging_tcp_config.cpp \
    src/shared/config/osd_cms_config.cpp \
    src/shared/utils/utils.cpp \
    src/usecase/osd/cms/osd_cms.cpp \
    src/usecase/osd/cms/osd_cms_input_mode.cpp \
    src/usecase/osd/cms/osd_cms_position_data.cpp \
    src/usecase/osd/osd_service.cpp \
    src/usecase/osd/stream/osd_stream.cpp \
    src/usecase/osd/stream/osd_stream_position.cpp \
    src/view/mainwindow.cpp \
    src/view/panel/gun/framegun.cpp \
    src/view/panel/osd/frame_osd_gyro.cpp \
    src/view/panel/osd/frame_osd_mode.cpp \
    src/view/panel/osd/frame_osd_position.cpp \
    src/view/panel/osd/frameosd.cpp \
    src/view/panel/wap/engagement_data/frame_engage_data.cpp \
    src/view/panel/wap/framewap.cpp \
    src/view/panel/wap/gun_correction/frame_gun_correction.cpp \
    src/view/panel/wap/track_engagement/frame_track_engage.cpp \
    src/view/panel/wap/weapon_assign/frame_weapon_assign.cpp \
    src/view/shared/frame_text_input.cpp \
    src/view/tda/components/tda_compass_object.cpp \
    src/view/tda/components/tda_object_base.cpp \
    src/view/tda/components/track/track.cpp \
    src/view/tda/frametda.cpp \
    src/view/tda/rceventhandler.cpp \
    src/view/tda/tda_global.cpp \
    test/shared/config/test_osd_cms_config.cpp \
    test/shared/errors/test_base_error.cpp \
    test/shared/utils/test_utils.cpp \
    test/testsuit.cpp \
    test/usecase/osd/cms/test_usecase_osd_cms.cpp

HEADERS += \
    src/di/di.h \
    src/infra/http/http_client_wrapper.h \
    src/infra/messaging/tcp/tcp_messaging_wrapper.h \
    src/model/base_request.h \
    src/model/base_response.h \
    src/model/osd/cms/osd_input_mode_request.h \
    src/model/osd/cms/osd_set_position_request.h \
    src/model/osd/input_mode_model.h \
    src/model/osd/position_model.h \
    src/shared/common/errors/err_amqp.h \
    src/shared/common/errors/err_base.h \
    src/shared/common/errors/err_http.h \
    src/shared/common/errors/err_json_parse.h \
    src/shared/common/errors/err_messaging.h \
    src/shared/common/errors/err_object_creation.h \
    src/shared/common/errors/err_open_file.h \
    src/shared/common/errors/err_osd_data.h \
    src/shared/common/errors/helper_err.h \
    src/shared/config/amqp_config.h \
    src/shared/config/base_config.h \
    src/shared/config/configuration.h \
    src/shared/config/messaging_tcp_config.h \
    src/shared/config/osd_cms_config.h \
    src/shared/utils/utils.h \
    src/usecase/osd/cms/i_osd_cms.h \
    src/usecase/osd/cms/osd_cms.h \
    src/usecase/osd/cms/osd_cms_input_mode.h \
    src/usecase/osd/cms/osd_cms_position_data.h \
    src/usecase/osd/osd_service.h \
    src/usecase/osd/stream/IOSDStream.h \
    src/usecase/osd/stream/osd_stream.h \
    src/usecase/osd/stream/osd_stream_position.h \
    src/view/mainwindow.h \
    src/view/panel/gun/framegun.h \
    src/view/panel/osd/frame_osd_base.h \
    src/view/panel/osd/frame_osd_gyro.h \
    src/view/panel/osd/frame_osd_mode.h \
    src/view/panel/osd/frame_osd_position.h \
    src/view/panel/osd/frameosd.h \
    src/view/panel/wap/engagement_data/frame_engage_data.h \
    src/view/panel/wap/framewap.h \
    src/view/panel/wap/gun_correction/frame_gun_correction.h \
    src/view/panel/wap/track_engagement/frame_track_engage.h \
    src/view/panel/wap/weapon_assign/frame_weapon_assign.h \
    src/view/shared/constant_ui.h \
    src/view/shared/frame_text_input.h \
    src/view/tda/components/tda_compass_object.h \
    src/view/tda/components/tda_object_base.h \
    src/view/tda/components/track/track.h \
    src/view/tda/frametda.h \
    src/view/tda/rceventhandler.h \
    src/view/tda/tda_global.h \
    test/shared/config/test_osd_cms_config.h \
    test/shared/errors/test_base_error.h \
    test/shared/utils/test_utils.h \
    test/testsuit.h \
    test/usecase/osd/cms/test_usecase_osd_cms.h

FORMS += \
    src/view/mainwindow.ui \
    src/view/panel/gun/framegun.ui \
    src/view/panel/osd/frame_osd_gyro.ui \
    src/view/panel/osd/frame_osd_mode.ui \
    src/view/panel/osd/frame_osd_position.ui \
    src/view/panel/osd/frameosd.ui \
    src/view/panel/wap/engagement_data/frame_engage_data.ui \
    src/view/panel/wap/framewap.ui \
    src/view/panel/wap/gun_correction/frame_gun_correction.ui \
    src/view/panel/wap/track_engagement/frame_track_engage.ui \
    src/view/panel/wap/weapon_assign/frame_weapon_assign.ui \
    src/view/shared/frame_text_input.ui \
    src/view/tda/frametda.ui

contains(DEFINES, USE_AMQP) {
    HEADERS += \
        src/infra/messaging/amqp/amqp_consumer_base.h \
        src/infra/messaging/amqp/amqp_consumer_wrapper.h \
        src/infra/messaging/amqp/amqp_options.h

    SOURCES += \
        src/infra/messaging/amqp/amqp_consumer_wrapper.cpp \
        src/infra/messaging/amqp/amqp_options.cpp

    unix:!macx: LIBS += -lqamqp
}

RESOURCES += \
    src/view/assets/hmi_fa_fcs.qrc
