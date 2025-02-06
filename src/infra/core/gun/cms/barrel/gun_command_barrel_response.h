#ifndef GUNCOMMANDBARRELRESPONSE_H
#define GUNCOMMANDBARRELRESPONSE_H

#include <QJsonObject>

class GunCommandBarrelResponse
{
public:
    GunCommandBarrelResponse();
    GunCommandBarrelResponse(float azimuth, float elevation);

    static GunCommandBarrelResponse FromJsonObject(QJsonObject obj);

    float getAzimuth() const;
    float getElevation() const;

private:
    float azimuth;
    float elevation;
};

#endif // GUNCOMMANDBARRELRESPONSE_H
