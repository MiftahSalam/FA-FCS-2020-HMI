#ifndef UTILS_H
#define UTILS_H

#include <QByteArray>
#include <QJsonObject>

class Utils
{
public:
    static QJsonObject byteArrayToJsonObject(QByteArray raw);
    static QString latDecToStringDegree(const float lat);
};

#endif // UTILS_H
