QT       += core gui network testlib serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#include($$PWD/qredis/qredis.pri)
include(src.pri)

#DEFINES += USE_AMQP
DEFINES += USE_LOG4QT

TARGET = fa_fcs_hmi
TEMPLATE = app

INCLUDEPATH += .. .

header_domain.files = $$HEADERS_DOMAIN
