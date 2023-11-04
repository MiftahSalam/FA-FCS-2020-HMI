#ifndef UTILS_H
#define UTILS_H

#include <QByteArray>
#include <QJsonObject>

class Utils
{
public:
    static QJsonObject byteArrayToJsonObject(QByteArray raw);
};

#endif // UTILS_H
