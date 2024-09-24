#include "tcp_messaging_wrapper.h"
#include "src/shared/common/errors/err_messaging.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/common/errors/err_messaging.h"

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
//    qDebug()<<Q_FUNC_INFO<<"state"<<consumer->state();

    if (consumer->state() != QTcpSocket::ConnectedState && consumer->state() != QTcpSocket::ConnectingState) {
        consumer->connectToHost(config->ip, config->port);
    }

    QDateTime now = QDateTime::currentDateTime();
    auto deltaDelay = timestampDelay.msecsTo(now);
    auto deltaHB = timestampHeartBeat.msecsTo(now);
    if (consumer->state() != QTcpSocket::ConnectedState) {
        return ErrMessagingNotConnected();
    } else if (deltaHB > config->timeout) {
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
        qDebug()<<Q_FUNC_INFO<<"delta"<<delta<<"delay"<<config->delay;
        if ( delta > config->delay) {
            timestampDelay = now;
            emit signalForwardMessage(data);
        }
    }

    qDebug()<<Q_FUNC_INFO<<"data"<<data;
}
