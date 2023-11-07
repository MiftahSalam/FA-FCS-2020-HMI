#ifndef AMQP_CONSUMER_BASE_H
#define AMQP_CONSUMER_BASE_H

#include <QString>

class AmqpConsumerBase
{
public:
    virtual void Connect() = 0;
    virtual void Check() = 0;
    virtual void Start(QString topic) = 0;

protected:

};
#endif // AMQP_CONSUMER_BASE_H
