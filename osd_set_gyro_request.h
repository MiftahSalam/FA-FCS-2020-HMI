#ifndef OSDSETGYROREQUEST_H
#define OSDSETGYROREQUEST_H

#include <QObject>

class OSDSetGyroRequest : public QObject
{
    Q_OBJECT
public:
    explicit OSDSetGyroRequest(QObject *parent = nullptr);

signals:

};

#endif // OSDSETGYROREQUEST_H
