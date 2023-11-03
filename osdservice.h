#ifndef OSDSERVICE_H
#define OSDSERVICE_H

#include <QObject>

class OSDService : public QObject
{
    Q_OBJECT
public:
    explicit OSDService(QObject *parent = nullptr);

signals:

};

#endif // OSDSERVICE_H
