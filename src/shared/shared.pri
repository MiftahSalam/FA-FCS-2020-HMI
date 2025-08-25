HEADERS += \
           $$PWD/global.h \
           $$PWD/common/errors/err_amqp.h \
           $$PWD/common/errors/err_base.h \
           $$PWD/common/errors/err_engagement.h \
           $$PWD/common/errors/err_json_parse.h \
           $$PWD/common/errors/err_messaging.h \
           $$PWD/common/errors/err_object_creation.h \
           $$PWD/common/errors/err_open_file.h \
           $$PWD/common/errors/err_osd_data.h \
           $$PWD/common/errors/helper_err.h \
           $$PWD/config/amqp_config.h \
           $$PWD/config/app_config.h \
           $$PWD/config/arpa_config.h \
           $$PWD/config/base_config.h \
           $$PWD/config/configuration.h \
           $$PWD/config/gun_cms_config.h \
           $$PWD/config/messaging_serial_config.h \
           $$PWD/config/messaging_tcp_config.h \
           $$PWD/config/osd_cms_config.h \
           $$PWD/config/tda_config.h \
           $$PWD/config/track_weapon_assign_config.h \
           $$PWD/utils/utils.h \
           $$PWD/utils/track/track_utils.h

SOURCES += \
           $$PWD/global.h \
           $$PWD/common/errors/err_amqp.cpp \
           $$PWD/common/errors/err_base.cpp \
           $$PWD/common/errors/err_engagement.cpp \
           $$PWD/common/errors/err_json_parse.cpp \
           $$PWD/common/errors/err_messaging.cpp \
           $$PWD/common/errors/err_object_creation.cpp \
           $$PWD/common/errors/err_open_file.cpp \
           $$PWD/common/errors/err_osd_data.cpp \
           $$PWD/common/errors/helper_err.cpp \
           $$PWD/config/amqp_config.cpp \
           $$PWD/config/app_config.cpp \
           $$PWD/config/arpa_config.cpp \
           $$PWD/config/configuration.cpp \
           $$PWD/config/gun_cms_config.cpp \
           $$PWD/config/messaging_serial_config.cpp \
           $$PWD/config/messaging_tcp_config.cpp \
           $$PWD/config/osd_cms_config.cpp \
           $$PWD/config/tda_config.cpp \
           $$PWD/config/track_weapon_assign_config.cpp \
           $$PWD/utils/utils.cpp \
           $$PWD/utils/track/track_utils.cpp
