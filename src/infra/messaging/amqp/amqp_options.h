#ifndef AMQPOPTIONS_H
#define AMQPOPTIONS_H

#include <QString>
#include <QStringList>

class AMQPOptions
{
public:
    AMQPOptions(const QString &url, const QString &name, const QString &exchange, const QString &exchangeType, const QString &routingKey, const QStringList &queue);

    QString getUrl() const;
    QString getName() const;
    QString getExchange() const;
    QString getExchangeType() const;
    QString getRoutingKey() const;
    QStringList getQueue() const;

private:
    QString url;
    QString name;
    QString exchange;
    QString exchangeType;
    QString routingKey;
    QStringList queue;
};

#endif // AMQPOPTIONS_H
