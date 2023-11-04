#ifndef POSITIONMODEL_H
#define POSITIONMODEL_H

#include <QJsonObject>

class PositionModel
{
public:
    PositionModel(double lat, double lon);

    static PositionModel fromJsonObject(QJsonObject obj);

    double getLatitude() const;
    double getLongitude() const;

private:
    double latitude, longitude;

};

#endif // POSITIONMODEL_H
