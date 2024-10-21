#include "tcp_messaging_wrapper.h"
#include "src/shared/common/errors/err_messaging.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/common/errors/err_messaging.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, TcpMessagingWrapper)
#else
#include <QDebug>
#endif

TcpMessagingWrapper::TcpMessagingWrapper(QObject *parent, TcpMessagingOpts *cfg)
    : QObject{parent}, config(cfg), currentError(0, "")
{
    if (cfg == nullptr) {
        throw ErrObjectCreation();
    }

    currentError = NoError();

    consumer = new QTcpSocket(this);
    connect(consumer, &QTcpSocket::connected, this, &TcpMessagingWrapper::onConnected);
    connect(consumer, &QTcpSocket::disconnected, this, &TcpMessagingWrapper::onDisconnected);

    QDateTime now = QDateTime::currentDateTime();
    timestampDelay = now;
    timestampHeartBeat = now;
}

BaseError TcpMessagingWrapper::checkConnection()
{
    if (consumer->state() != QTcpSocket::ConnectedState && consumer->state() != QTcpSocket::ConnectingState) {
        consumer->connectToHost(config->ip, config->port);
    }

    QDateTime now = QDateTime::currentDateTime();
    auto deltaDelay = timestampDelay.msecsTo(now);
    auto deltaHB = timestampHeartBeat.msecsTo(now);
    if (consumer->state() != QTcpSocket::ConnectedState) {
        return ErrMessagingNotConnected();
    } else if (deltaHB > config->timeout) {
        if (deltaHB > 5 * config->timeout) {
            timestampHeartBeat = now;
            consumer->disconnectFromHost();
#ifdef USE_LOG4QT
            logger()->info()<<Q_FUNC_INFO<<" reconnecting to ip: "<<config->ip<<", port: "<<config->port;
#else
            qDebug<<Q_FUNC_INFO<<" reconnecting to ip: "<<config->ip<<", port: "<<config->port;
#endif
        }
        return ErrMessagingDataNoData();
    } else if (deltaDelay > config->timeout) {
        return ErrMessagingDataInvalidFormat();
    } else {
        return NoError();
    }
}

void TcpMessagingWrapper::sendMessage(QByteArray data)
{
    consumer->write(data);
}

void TcpMessagingWrapper::onConnected()
{
    connect(consumer, &QTcpSocket::readyRead, this, &TcpMessagingWrapper::onReadyRead);
}

void TcpMessagingWrapper::onDisconnected()
{
    disconnect(consumer, &QTcpSocket::readyRead, this, &TcpMessagingWrapper::onReadyRead);
}

void TcpMessagingWrapper::onReadyRead()
{
    QDateTime now = QDateTime::currentDateTime();
    timestampHeartBeat = now;

    QByteArray data = consumer->readAll();
    if (data.startsWith("{") && data.endsWith("}")) {
        auto delta = timestampDelay.msecsTo(now);
#ifdef USE_LOG4QT
        logger()->trace()<<Q_FUNC_INFO<<" -> delta; "<<delta<<", delay: "<<config->delay;
#else
        qDebug()<<Q_FUNC_INFO<<"delta"<<delta<<"delay"<<config->delay;
#endif
        if ( delta > config->delay) {
            timestampDelay = now;
            emit signalForwardMessage(data);
        }
    }

#ifdef USE_LOG4QT
    logger()->trace()<<Q_FUNC_INFO<<" -> data; "<<data;
#else
    qDebug()<<Q_FUNC_INFO<<"data"<<data;
#endif
}
