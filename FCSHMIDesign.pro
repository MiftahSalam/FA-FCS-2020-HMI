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
    src/domain/engagement/entity/engagement_data_correction_entity.cpp \
    src/domain/engagement/entity/engagement_data_entity.cpp \
    src/domain/engagement/entity/weapon_track_assign_entity.cpp \
    src/domain/fire_triangle/entity/fire_triangle_entity.cpp \
    src/domain/gun/entity/gun_barrel_entity.cpp \
    src/domain/gun/entity/gun_barrel_mode_entity.cpp \
    src/domain/gun/entity/gun_coverage_entity.cpp \
    src/domain/gun/entity/gun_status_command_entity.cpp \
    src/domain/gun/entity/gun_status_feedback_entity.cpp \
    src/domain/osd/entity/osd_base_entity.cpp \
    src/domain/osd/entity/osd_inertia_entity.cpp \
    src/domain/osd/entity/osd_position_entity.cpp \
    src/domain/osd/entity/osd_waterspeed_entity.cpp \
    src/domain/osd/entity/osd_speed_entity.cpp \
    src/domain/osd/entity/osd_weather_entity.cpp \
    src/domain/osd/entity/osd_wind_entity.cpp \
    src/domain/track/entity/track_base_entity.cpp \
    src/domain/weapon_assign/entity/weapon_assign_entity.cpp \
    src/infra/http/http_client_wrapper.cpp \
    src/infra/messaging/tcp/tcp_messaging_wrapper.cpp \
    src/infra/store/engagement/inmemory/engagement_data_correction_repository_inmem_impl.cpp \
    src/infra/store/engagement/inmemory/engagement_data_repository_inmem_impl.cpp \
    src/infra/store/fire_triangle/fire_triangle_repository.cpp \
    src/infra/store/fire_triangle/inmemory/fire_triangle_repository_inmem_impl.cpp \
    src/infra/store/gun/gun_repository.cpp \
    src/infra/store/gun/inmemory/gun_command_repository_inmem_impl.cpp \
    src/infra/store/gun/inmemory/gun_feedback_repository_inmem_imp.cpp \
    src/infra/store/gun/inmemory/gun_coverage_repository_inmem_impl.cpp \
    src/infra/store/osd/inmemory/osd_inertia_repository_inmem_impl.cpp \
    src/infra/store/osd/inmemory/osd_position_repository_inmem_impl.cpp \
    src/infra/store/osd/inmemory/osd_waterspeed_repository_inmem_impl.cpp \
    src/infra/store/osd/inmemory/osd_speed_repository_inmem_impl.cpp \
    src/infra/store/osd/inmemory/osd_weather_repository_inmem_impl.cpp \
    src/infra/store/osd/inmemory/osd_wind_repository_inmem_impl.cpp \
    src/infra/store/osd/osd_repository.cpp \
    src/infra/store/track/inmem/track_arpa_repository_inmem_impl.cpp \
    src/infra/store/track/track_repository.cpp \
    src/infra/store/weapon_assign/inmemory/weapon_assignment_repository_inmem_impl.cpp \
    src/infra/store/weapon_track_assign/weapon_track_assignment_repository_inmem_impl.cpp \
    src/main.cpp \
    src/model/engagement/cms/engagement_correction_set_request.cpp \
    src/model/engagement/engagement_data_model.cpp \
    src/model/gun/cms/gun_command_barrel_request.cpp \
    src/model/gun/cms/gun_command_barrel_response.cpp \
    src/model/gun/cms/gun_mode_barrel_request.cpp \
    src/model/gun/cms/gun_mode_barrel_response.cpp \
    src/model/gun/gun_coverage_model.cpp \
    src/model/gun/gun_feedback_barrel_model.cpp \
    src/model/gun/gun_feedback_status_model.cpp \
    src/model/osd/cms/osd_input_mode_request.cpp \
    src/model/osd/cms/osd_set_gyro_request.cpp \
    src/model/osd/cms/osd_set_position_request.cpp \
    src/model/osd/cms/osd_set_waterspeed_request.cpp \
    src/model/osd/cms/osd_set_speed_request.cpp \
    src/model/osd/cms/osd_set_weather_request.cpp \
    src/model/osd/cms/osd_set_wind_request.cpp \
    src/model/osd/gyro_model.cpp \
    src/model/osd/input_mode_model.cpp \
    src/model/osd/position_model.cpp \
    src/model/osd/waterspeed_model.cpp \
    src/model/osd/speed_model.cpp \
    src/model/osd/weather_model.cpp \
    src/model/osd/wind_model.cpp \
    src/model/track/arpa/track_arpa_model.cpp \
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
    src/shared/config/arpa_config.cpp \
    src/shared/config/configuration.cpp \
    src/shared/config/gun_cms_config.cpp \
    src/shared/config/messaging_tcp_config.cpp \
    src/shared/config/osd_cms_config.cpp \
    src/shared/config/tda_config.cpp \
    src/shared/utils/track/track_utils.cpp \
    src/shared/utils/utils.cpp \
    src/usecase/engagement/cms/engagement_data_correction_40mm_service.cpp \
    src/usecase/engagement/stream/engagement_data_40mm_stream.cpp \
    src/usecase/engagement/weapon_track_assign_service.cpp \
    src/usecase/gun/cms/gun_command_barrel_mode_service.cpp \
    src/usecase/gun/cms/gun_command_barrel_service.cpp \
    src/usecase/gun/gun_manager_service.cpp \
    src/usecase/gun/stream/gun_coverage_stream.cpp \
    src/usecase/gun/stream/gun_feedback_barrel_stream.cpp \
    src/usecase/gun/stream/gun_feedback_status_stream.cpp \
    src/usecase/gun/stream/gun_stream.cpp \
    src/usecase/osd/cms/osd_cms.cpp \
    src/usecase/osd/cms/osd_cms_gyro_data.cpp \
    src/usecase/osd/cms/osd_cms_input_mode.cpp \
    src/usecase/osd/cms/osd_cms_position_data.cpp \
    src/usecase/osd/cms/osd_cms_waterspeed_data.cpp \
    src/usecase/osd/cms/osd_cms_speed_data.cpp \
    src/usecase/osd/cms/osd_cms_weather_data.cpp \
    src/usecase/osd/cms/osd_cms_wind_data.cpp \
    src/usecase/osd/osd_service.cpp \
    src/usecase/osd/stream/osd_stream.cpp \
    src/usecase/osd/stream/osd_stream_gyro.cpp \
    src/usecase/osd/stream/osd_stream_position.cpp \
    src/usecase/osd/stream/osd_stream_waterspeed.cpp \
    src/usecase/osd/stream/osd_stream_speed.cpp \
    src/usecase/osd/stream/osd_stream_weather.cpp \
    src/usecase/osd/stream/osd_stream_wind.cpp \
    src/usecase/track/stream/arpa/track_arpa_stream.cpp \
    src/usecase/track/stream/track_stream.cpp \
    src/usecase/weapon_assign/weapon_assign_service.cpp \
    src/view/mainwindow.cpp \
    src/view/panel/gun/frame_gun_control_barrel.cpp \
    src/view/panel/gun/frame_gun_control_fire.cpp \
    src/view/panel/gun/frame_gun_control_status.cpp \
    src/view/panel/gun/frame_gun_data_monitoring.cpp \
    src/view/panel/gun/framegun.cpp \
    src/view/panel/osd/frame_osd_gyro.cpp \
    src/view/panel/osd/frame_osd_mode.cpp \
    src/view/panel/osd/frame_osd_position.cpp \
    src/view/panel/osd/frame_osd_waterspeed.cpp \
    src/view/panel/osd/frame_osd_speed.cpp \
    src/view/panel/osd/frame_osd_weather.cpp \
    src/view/panel/osd/frame_osd_wind.cpp \
    src/view/panel/osd/frameosd.cpp \
    src/view/panel/wap/frame_engage_data.cpp \
    src/view/panel/wap/framewap.cpp \
    src/view/panel/wap/frame_gun_correction.cpp \
    src/view/panel/wap/frame_track_engage.cpp \
    src/view/panel/wap/frame_weapon_assign.cpp \
    src/view/shared/frame_text_input.cpp \
    src/view/tda/components/tda_compass_object.cpp \
    src/view/tda/components/tda_fire_triangle_object.cpp \
    src/view/tda/components/tda_gun_barrel_object.cpp \
    src/view/tda/components/tda_gun_coverage_object.cpp \
    src/view/tda/components/tda_heading_marker_object.cpp \
    src/view/tda/components/tda_object_base.cpp \
    src/view/tda/components/track/tda_track.cpp \
    src/view/tda/components/track/tda_track_icon.cpp \
    src/view/tda/components/track/tda_track_id.cpp \
    src/view/tda/components/track/tda_tracks_object.cpp \
    src/view/tda/components/track/track_param.cpp \
    src/view/tda/events/tda_event_filter.cpp \
    src/view/tda/frametda.cpp \
    test/shared/config/test_osd_cms_config.cpp \
    test/shared/errors/test_base_error.cpp \
    test/shared/utils/test_utils.cpp \
    test/testsuit.cpp \
    test/usecase/osd/cms/test_usecase_osd_cms.cpp

HEADERS += \
    src/di/di.h \
    src/domain/engagement/entity/engagement_data_correction_entity.h \
    src/domain/engagement/entity/engagement_data_entity.h \
    src/domain/engagement/entity/weapon_track_assign_entity.h \
    src/domain/engagement/repository/engagement_data_correction_repository.h \
    src/domain/engagement/repository/engagement_data_repository.h \
    src/domain/engagement/repository/weapon_track_assign_repository.h \
    src/domain/fire_triangle/entity/fire_triangle_entity.h \
    src/domain/fire_triangle/repository/fire_triangle_repository.h \
    src/domain/gun/entity/gun_barrel_entity.h \
    src/domain/gun/entity/gun_barrel_mode_entity.h \
    src/domain/gun/entity/gun_coverage_entity.h \
    src/domain/gun/entity/gun_status_command_entity.h \
    src/domain/gun/entity/gun_status_feedback_entity.h \
    src/domain/gun/repository/gun_base_repository.h \
    src/domain/gun/repository/gun_command_repository.h \
    src/domain/gun/repository/gun_coverage_repository.h \
    src/domain/gun/repository/gun_feedback_repository.h \
    src/domain/osd/entity/osd_base_entity.h \
    src/domain/osd/entity/osd_inertia_entity.h \
    src/domain/osd/entity/osd_position_entity.h \
    src/domain/osd/entity/osd_waterspeed_entity.h \
    src/domain/osd/entity/osd_weather_entity.h \
    src/domain/osd/entity/osd_wind_entity.h \
    src/domain/osd/repository/osd_base_repository.h \
    src/domain/osd/repository/osd_inertia_repository.h \
    src/domain/osd/repository/osd_position_repository.h \
    src/domain/osd/repository/osd_waterspeed_repository.h \
    src/domain/osd/entity/osd_speed_entity.h \
    src/domain/osd/repository/osd_base_repository.h \
    src/domain/osd/repository/osd_inertia_repository.h \
    src/domain/osd/repository/osd_position_repository.h \
    src/domain/osd/repository/osd_speed_repository.h \
    src/domain/osd/repository/osd_weather_repository.h \
    src/domain/osd/repository/osd_wind_repository.h \
    src/domain/track/entity/track_base_entity.h \
    src/domain/track/repository/track_base_repository.h \
    src/domain/weapon_assign/entity/weapon_assign_entity.h \
    src/domain/weapon_assign/repository/weapon_assignment_repository.h \
    src/infra/http/http_client_wrapper.h \
    src/infra/messaging/tcp/tcp_messaging_wrapper.h \
    src/infra/store/engagement/inmemory/engagement_data_correction_repository_inmem_impl.h \
    src/infra/store/engagement/inmemory/engagement_data_repository_inmem_impl.h \
    src/infra/store/fire_triangle/fire_triangle_repository.h \
    src/infra/store/fire_triangle/inmemory/fire_triangle_repository_inmem_impl.h \
    src/infra/store/gun/gun_repository.h \
    src/infra/store/gun/inmemory/gun_command_repository_inmem_impl.h \
    src/infra/store/gun/inmemory/gun_feedback_repository_inmem_imp.h \
    src/infra/store/gun/inmemory/gun_coverage_repository_inmem_impl.h \
    src/infra/store/osd/inmemory/osd_inertia_repository_inmem_impl.h \
    src/infra/store/osd/inmemory/osd_position_repository_inmem_impl.h \
    src/infra/store/osd/inmemory/osd_waterspeed_repository_inmem_impl.h \
    src/infra/store/osd/inmemory/osd_speed_repository_inmem_impl.h \
    src/infra/store/osd/inmemory/osd_weather_repository_inmem_impl.h \
    src/infra/store/osd/inmemory/osd_wind_repository_inmem_impl.h \
    src/infra/store/osd/osd_repository.h \
    src/infra/store/track/inmem/track_arpa_repository_inmem_impl.h \
    src/infra/store/track/track_repository.h \
    src/infra/store/track/track_repository_listener.h \
    src/infra/store/track/track_repository_publisher.h \
    src/infra/store/weapon_assign/inmemory/weapon_assignment_repository_inmem_impl.h \
    src/infra/store/weapon_track_assign/weapon_track_assignment_repository_inmem_impl.h \
    src/model/base_request.h \
    src/model/base_response.h \
    src/model/engagement/cms/engagement_correction_set_request.h \
    src/model/engagement/engagement_data_model.h \
    src/model/gun/cms/gun_command_barrel_request.h \
    src/model/gun/cms/gun_command_barrel_response.h \
    src/model/gun/cms/gun_mode_barrel_request.h \
    src/model/gun/cms/gun_mode_barrel_response.h \
    src/model/gun/gun_coverage_model.h \
    src/model/gun/gun_feedback_barrel_model.h \
    src/model/gun/gun_feedback_status_model.h \
    src/model/osd/cms/osd_input_mode_request.h \
    src/model/osd/cms/osd_set_gyro_request.h \
    src/model/osd/cms/osd_set_position_request.h \
    src/model/osd/cms/osd_set_waterspeed_request.h \
    src/model/osd/cms/osd_set_weather_request.h \
    src/model/osd/cms/osd_set_wind_request.h \
    src/model/osd/gyro_model.h \
    src/model/osd/input_mode_model.h \
    src/model/osd/position_model.h \
    src/model/osd/waterspeed_model.h \
    src/model/osd/cms/osd_set_speed_request.h \
    src/model/osd/gyro_model.h \
    src/model/osd/input_mode_model.h \
    src/model/osd/position_model.h \
    src/model/osd/speed_model.h \
    src/model/osd/weather_model.h \
    src/model/osd/wind_model.h \
    src/model/track/arpa/track_arpa_model.h \
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
    src/shared/config/arpa_config.h \
    src/shared/config/base_config.h \
    src/shared/config/configuration.h \
    src/shared/config/gun_cms_config.h \
    src/shared/config/messaging_tcp_config.h \
    src/shared/config/osd_cms_config.h \
    src/shared/config/tda_config.h \
    src/shared/utils/track/track_utils.h \
    src/shared/utils/utils.h \
    src/usecase/engagement/cms/engagement_data_correction_40mm_service.h \
    src/usecase/engagement/stream/engagement_data_40mm_stream.h \
    src/usecase/engagement/stream/engagement_data_base_stream.h \
    src/usecase/engagement/weapon_track_assign_service.h \
    src/usecase/gun/cms/gun_command_barrel_mode_service.h \
    src/usecase/gun/cms/gun_command_barrel_service.h \
    src/usecase/gun/gun_manager_service.h \
    src/usecase/gun/stream/gun_coverage_stream.h \
    src/usecase/gun/stream/gun_feedback_barrel_stream.h \
    src/usecase/gun/stream/gun_feedback_status_stream.h \
    src/usecase/gun/stream/gun_stream.h \
    src/usecase/gun/stream/gun_stream_base.h \
    src/usecase/osd/cms/i_osd_cms.h \
    src/usecase/osd/cms/osd_cms.h \
    src/usecase/osd/cms/osd_cms_gyro_data.h \
    src/usecase/osd/cms/osd_cms_input_mode.h \
    src/usecase/osd/cms/osd_cms_position_data.h \
    src/usecase/osd/cms/osd_cms_waterspeed_data.h \
    src/usecase/osd/cms/osd_cms_speed_data.h \
    src/usecase/osd/cms/osd_cms_weather_data.h \
    src/usecase/osd/cms/osd_cms_wind_data.h \
    src/usecase/osd/osd_service.h \
    src/usecase/osd/stream/IOSDStream.h \
    src/usecase/osd/stream/osd_stream.h \
    src/usecase/osd/stream/osd_stream_gyro.h \
    src/usecase/osd/stream/osd_stream_position.h \
    src/usecase/osd/stream/osd_stream_waterspeed.h \
    src/usecase/osd/stream/osd_stream_speed.h \
    src/usecase/osd/stream/osd_stream_weather.h \
    src/usecase/osd/stream/osd_stream_wind.h \
    src/usecase/track/stream/arpa/track_arpa_stream.h \
    src/usecase/track/stream/track_stream.h \
    src/usecase/track/stream/track_stream_base.h \
    src/usecase/weapon_assign/weapon_assign_service.h \
    src/view/mainwindow.h \
    src/view/panel/gun/frame_gun_base.h \
    src/view/panel/gun/frame_gun_control_barrel.h \
    src/view/panel/gun/frame_gun_control_fire.h \
    src/view/panel/gun/frame_gun_control_status.h \
    src/view/panel/gun/frame_gun_data_monitoring.h \
    src/view/panel/gun/framegun.h \
    src/view/panel/osd/frame_osd_base.h \
    src/view/panel/osd/frame_osd_gyro.h \
    src/view/panel/osd/frame_osd_mode.h \
    src/view/panel/osd/frame_osd_position.h \
    src/view/panel/osd/frame_osd_waterspeed.h \
    src/view/panel/osd/frame_osd_speed.h \
    src/view/panel/osd/frame_osd_weather.h \
    src/view/panel/osd/frame_osd_wind.h \
    src/view/panel/osd/frameosd.h \
    src/view/panel/wap/frame_engage_data.h \
    src/view/panel/wap/framewap.h \
    src/view/panel/wap/frame_gun_correction.h \
    src/view/panel/wap/frame_track_engage.h \
    src/view/panel/wap/frame_weapon_assign.h \
    src/view/shared/constant_ui.h \
    src/view/shared/frame_text_input.h \
    src/view/tda/components/tda_compass_object.h \
    src/view/tda/components/tda_fire_triangle_object.h \
    src/view/tda/components/tda_gun_barrel_object.h \
    src/view/tda/components/tda_gun_coverage_object.h \
    src/view/tda/components/tda_heading_marker_object.h \
    src/view/tda/components/tda_object_base.h \
    src/view/tda/components/track/tda_track.h \
    src/view/tda/components/track/tda_track_icon.h \
    src/view/tda/components/track/tda_track_id.h \
    src/view/tda/components/track/tda_tracks_object.h \
    src/view/tda/components/track/track_param.h \
    src/view/tda/events/tda_event_filter.h \
    src/view/tda/frametda.h \
    test/shared/config/test_osd_cms_config.h \
    test/shared/errors/test_base_error.h \
    test/shared/utils/test_utils.h \
    test/testsuit.h \
    test/usecase/osd/cms/test_usecase_osd_cms.h

FORMS += \
    src/view/mainwindow.ui \
    src/view/panel/gun/frame_gun_control_barrel.ui \
    src/view/panel/gun/frame_gun_control_fire.ui \
    src/view/panel/gun/frame_gun_control_status.ui \
    src/view/panel/gun/frame_gun_data_monitoring.ui \
    src/view/panel/gun/framegun.ui \
    src/view/panel/osd/frame_osd_gyro.ui \
    src/view/panel/osd/frame_osd_mode.ui \
    src/view/panel/osd/frame_osd_position.ui \
    src/view/panel/osd/frame_osd_waterspeed.ui \
    src/view/panel/osd/frame_osd_speed.ui \
    src/view/panel/osd/frame_osd_weather.ui \
    src/view/panel/osd/frame_osd_wind.ui \
    src/view/panel/osd/frameosd.ui \
    src/view/panel/wap/frame_engage_data.ui \
    src/view/panel/wap/framewap.ui \
    src/view/panel/wap/frame_gun_correction.ui \
    src/view/panel/wap/frame_track_engage.ui \
    src/view/panel/wap/frame_weapon_assign.ui \
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

QMAKE_LFLAGS += -no-pie
