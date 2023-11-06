#include "amqp_options.h"

AMQPOptions::AMQPOptions(const QString &url, const QString &name, const QString &exchange, const QString &exchangeType, const QString &routingKey, const QStringList &queue) : url(url),
    name(name),
    exchange(exchange),
    exchangeType(exchangeType),
    routingKey(routingKey),
    queue(queue)
{}

QString AMQPOptions::getUrl() const
{
    return url;
}

QString AMQPOptions::getName() const
{
    return name;
}

QString AMQPOptions::getExchange() const
{
    return exchange;
}

QString AMQPOptions::getExchangeType() const
{
    return exchangeType;
}

QString AMQPOptions::getRoutingKey() const
{
    return routingKey;
}

QStringList AMQPOptions::getQueue() const
{
    return queue;
}
