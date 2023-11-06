#include "amqp_consumer_wrapper.h"
#include "src/shared/common/errors/err_amqp.h"

#include <qamqp/qamqpqueue.h>

AmqpConsumerWrapper::AmqpConsumerWrapper(QObject *parent, AMQPOptions *opts)
    : QObject{parent}, config(opts)
{
    client = new QAmqpClient(this);
    client->setAutoReconnect(true, 1);
    connect(client, &QAmqpClient::connected, this, &AmqpConsumerWrapper::onConnected);
}

void AmqpConsumerWrapper::Connect()
{
    qDebug()<<Q_FUNC_INFO<<"url:"<<config->getUrl();
    client->connectToHost(config->getUrl());
}

void AmqpConsumerWrapper::Reconnect()
{
}

void AmqpConsumerWrapper::Start(QString topic)
{
}

void AmqpConsumerWrapper::onConnected()
{
    qDebug()<<Q_FUNC_INFO;

    QAmqpExchange *exchange = client->createExchange(config->getExchange());
    connect(exchange, &QAmqpExchange::declared, this, &AmqpConsumerWrapper::onExchangeDeclared);
    exchange->declare(stringToExchangeType(config->getExchangeType()));

    QAmqpQueue *queue = client->createQueue();
    connect(queue, &QAmqpQueue::declared, this, &AmqpConsumerWrapper::onQueueDeclared);
    connect(queue, &QAmqpQueue::bound, this, &AmqpConsumerWrapper::onQueueBounded);
    connect(queue, &QAmqpQueue::messageReceived, this, &AmqpConsumerWrapper::onMessageReceived);
    queue->declare();

}

void AmqpConsumerWrapper::onExchangeDeclared()
{
    qDebug()<<Q_FUNC_INFO;

    QAmqpQueue *queue = client->createQueue();
    connect(queue, &QAmqpQueue::declared, this, &AmqpConsumerWrapper::onQueueDeclared);
    connect(queue, &QAmqpQueue::bound, this, &AmqpConsumerWrapper::onQueueBounded);
    connect(queue, &QAmqpQueue::messageReceived, this, &AmqpConsumerWrapper::onMessageReceived);
    queue->declare();
}

void AmqpConsumerWrapper::onQueueDeclared()
{
    qDebug()<<Q_FUNC_INFO;

    QAmqpQueue *queue = qobject_cast<QAmqpQueue*>(sender());
    if (!queue) return;

    queue->bind(config->getExchange(), config->getRoutingKey());

}

void AmqpConsumerWrapper::onQueueBounded()
{
    qDebug()<<Q_FUNC_INFO;

    QAmqpQueue *queue = qobject_cast<QAmqpQueue*>(sender());
    if (!queue) return;

    queue->consume(QAmqpQueue::coNoAck);
}

void AmqpConsumerWrapper::onMessageReceived()
{
    QAmqpQueue *queue = qobject_cast<QAmqpQueue*>(sender());
    if (!queue) return;

    QAmqpMessage msg = queue->dequeue();

    qDebug()<<Q_FUNC_INFO<<"message from"<<msg.routingKey()<<":"<<msg.payload();

    emit signalForwardMessage(msg.payload());
}

QAmqpExchange::ExchangeType AmqpConsumerWrapper::stringToExchangeType(QString type)
{
    if (type == "topic") {
        return QAmqpExchange::Topic;
    } else if (type == "direct") {
        return QAmqpExchange::Direct;
    } else {
        throw ErrAmqpInvalidExchangeType();
    }
}
