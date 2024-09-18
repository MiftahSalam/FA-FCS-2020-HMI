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
    static int range2Pixel(double range, double max_range, int vp_width, int vp_height);
    static double pixel2Range(int pixel, double max_range, int vp_width, int vp_height);
    static QPoint polar2Cartesian(const double range, const double bearing, const double scale, const QPoint vp, const QPoint offset);
};

#endif // UTILS_H
