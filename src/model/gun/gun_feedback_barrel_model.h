#ifndef GUN_FEEDBACK_BARREL_MODEL_H
#define GUN_FEEDBACK_BARREL_MODEL_H

#include <QJsonObject>

class GunFeedbackBarrelModel
{
public:
    GunFeedbackBarrelModel(double azimuth, double elevation);
    GunFeedbackBarrelModel(){}

    static GunFeedbackBarrelModel fromJsonObject(QJsonObject obj);

    double getAzimuth() const;
    double getElevation() const;

private:
    double _azimuth, _elevation;

};

#endif // GUN_FEEDBACK_BARREL_MODEL_H
