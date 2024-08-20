#ifndef GUN_BARREL_MODEL_H
#define GUN_BARREL_MODEL_H

#include <QJsonObject>

class GunBarrelModel
{
public:
    GunBarrelModel(double azimuth, double elevation);
    GunBarrelModel(){}

    static GunBarrelModel fromJsonObject(QJsonObject obj);

    double getAzimuth() const;
    double getElevation() const;

private:
    double _azimuth, _elevation;

};

#endif // GUN_BARREL_MODEL_H
