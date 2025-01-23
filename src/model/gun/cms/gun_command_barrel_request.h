#ifndef GUNCOMMANDREQUEST_H
#define GUNCOMMANDREQUEST_H

#include "src/infra/core/base_request.h"

class GunCommandBarrelRequest: public BaseModel
{
public:
    GunCommandBarrelRequest();
    GunCommandBarrelRequest(float azimuth, float elevation);

    // BaseModel interface
    QByteArray toJSON() override;

    float getAzimuth() const;
    void setAzimuth(float newAzimuth);
    float getElevation() const;
    void setElevation(float newElevation);

private:
    float azimuth;
    float elevation;

};

#endif // GUNCOMMANDREQUEST_H
