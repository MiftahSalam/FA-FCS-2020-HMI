#ifndef GUNMODEBARRELRESPONSE_H
#define GUNMODEBARRELRESPONSE_H

#include "qjsonobject.h"

class GunModeBarrelResponse
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
