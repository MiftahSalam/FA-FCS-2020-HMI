#ifndef AMQPCONSUMERWRAPPER_H
#define AMQPCONSUMERWRAPPER_H

#include <QObject>

#include <qamqp/qamqpclient.h>
#include <qamqp/qamqpexchange.h>

#include "src/infra/messaging/amqp/amqp_consumer_base.h"
#include "src/infra/messaging/amqp/amqp_options.h"


class AmqpConsumerWrapper : public QObject, public AmqpConsumerBase
{
    Q_OBJECT
public:
    explicit AmqpConsumerWrapper(QObject *parent = nullptr, AMQPOptions *opts = nullptr);

    // AmqpConsumerBase interface
    void Connect() override;
    void Reconnect() override;
    void Start(QString topic) override;

private slots:
    void onConnected();
    void onExchangeDeclared();
    void onQueueDeclared();
    void onQueueBounded();
    void onMessageReceived();

signals:
    void signalForwardMessage(QByteArray msg);

private:
    AMQPOptions *config;
    QAmqpClient* client;

    QAmqpExchange::ExchangeType stringToExchangeType(QString type);
};

#endif // AMQPCONSUMERWRAPPER_H
