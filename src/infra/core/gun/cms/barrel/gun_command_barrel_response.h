#ifndef GUNCOMMANDBARRELRESPONSE_H
#define GUNCOMMANDBARRELRESPONSE_H

#include <QJsonObject>

#include "src/infra/core/gun/cms/gun_base_model.h"

class GunCommandBarrelResponse: public GunBaseModel
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
