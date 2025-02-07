#ifndef GUNMODEBARRELRESPONSE_H
#define GUNMODEBARRELRESPONSE_H

#include "qjsonobject.h"

#include "src/infra/core/gun/cms/gun_base_model.h"

class GunModeBarrelResponse: public GunBaseModel
{
public:
    GunModeBarrelResponse();
    GunModeBarrelResponse(bool manualMode);

    bool getManualMode() const;

    static GunModeBarrelResponse FromJsonObject(QJsonObject obj);

private:
    bool manualMode;
};

#endif // GUNMODEBARRELRESPONSE_H
