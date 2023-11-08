#ifndef UTILS_H
#define UTILS_H

#include <QByteArray>
#include <QJsonObject>

class Utils
{
public:
    static QJsonObject byteArrayToJsonObject(QByteArray raw);
    static QString latDecToStringDegree(const float lat);
    static float latStrToDegree(const QString lat);
    static QString lonDecToStringDegree(const float lon);
    static float lonStrToDegree(const QString lon);
};

#endif // UTILS_H
