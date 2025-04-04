#ifndef TCPMESSAGINGWRAPPER_H
#define TCPMESSAGINGWRAPPER_H

#include "src/shared/config/messaging_tcp_config.h"
#include "src/shared/common/errors/err_base.h"

#include <QDateTime>
#include <QObject>
#include <QTcpSocket>

class TcpMessagingWrapper : public QObject
{
    Q_OBJECT
public:
    explicit TcpMessagingWrapper(QObject *parent = nullptr, TcpMessagingOpts *cfg = nullptr);

    BaseError checkConnection();

    void sendMessage(QByteArray data);

signals:
    void signalForwardMessage(QByteArray msg);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();

private:
    QTcpSocket *consumer;
    TcpMessagingOpts *config;
    QDateTime timestampHeartBeat;
    QDateTime timestampDelay;
    BaseError currentError;
};

#endif // TCPMESSAGINGWRAPPER_H
