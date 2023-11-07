#include "amqp_consumer_wrapper.h"
#include "src/shared/common/errors/err_amqp.h"

#include <qamqp/qamqpqueue.h>

AmqpConsumerWrapper::AmqpConsumerWrapper(QObject *parent, AMQPOptions *opts)
    : QObject{parent}, config(opts), currentExchange(nullptr), currentQueue(nullptr), deleted(false)
{
    client = new QAmqpClient(this);
    client->setAutoReconnect(true);
//    connect(client, &QAmqpClient::disconnected, this, &AmqpConsumerWrapper::onDisconnected);
}

void AmqpConsumerWrapper::Connect()
{
    qDebug()<<Q_FUNC_INFO<<"url:"<<config->getUrl();
    connect(client, &QAmqpClient::connected, this, &AmqpConsumerWrapper::onConnected);
    client->connectToHost(config->getUrl());
}

void AmqpConsumerWrapper::Check()
{
//    qDebug()<<Q_FUNC_INFO<<"connected"<<client->isConnected();

    /*
    if (client == nullptr) {
        deleted = false;
        client = new QAmqpClient(this);
        connect(client, &QAmqpClient::connected, this, &AmqpConsumerWrapper::onConnected);
        connect(client, &QAmqpClient::disconnected, this, &AmqpConsumerWrapper::onDisconnected);
        Connect();
    }
    if (deleted) {
        deleted = false;

        delete client;

        client = nullptr;
    }
    */
    /*
    if (!client->isConnected()) {
        currentQueue->cancel();
        client->abort();
        Connect();
        currentExchange->reopen();
        currentQueue->reopen();
    } else {
        currentExchange = client->createExchange(config->getExchange());
        connect(currentExchange, &QAmqpExchange::declared, this, &AmqpConsumerWrapper::onExchangeDeclared);
        currentExchange->declare(stringToExchangeType(config->getExchangeType()));

        currentQueue = client->createQueue();
        connect(currentQueue, &QAmqpQueue::declared, this, &AmqpConsumerWrapper::onQueueDeclared);
        connect(currentQueue, &QAmqpQueue::bound, this, &AmqpConsumerWrapper::onQueueBounded);
        connect(currentQueue, &QAmqpQueue::messageReceived, this, &AmqpConsumerWrapper::onMessageReceived);
        currentQueue->declare();
    }
    */
}

void AmqpConsumerWrapper::Start(QString topic)
{
}

void AmqpConsumerWrapper::onConnected()
{
    qDebug()<<Q_FUNC_INFO;

    QAmqpExchange *exchange = client->createExchange(config->getExchange());
    disconnect(exchange, 0, 0, 0);
    connect(exchange, &QAmqpExchange::declared, this, &AmqpConsumerWrapper::onExchangeDeclared);
    exchange->declare(stringToExchangeType(config->getExchangeType()));

    QAmqpQueue *queue = client->createQueue();
    disconnect(queue, 0, 0, 0);
    connect(queue, &QAmqpQueue::declared, this, &AmqpConsumerWrapper::onQueueDeclared);
    connect(queue, &QAmqpQueue::bound, this, &AmqpConsumerWrapper::onQueueBounded);
    connect(queue, &QAmqpQueue::messageReceived, this, &AmqpConsumerWrapper::onMessageReceived);
    queue->declare();

}

void AmqpConsumerWrapper::onDisconnected()
{
    qDebug()<<Q_FUNC_INFO;
    deleted = true;
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

    queue->ack(msg);

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
