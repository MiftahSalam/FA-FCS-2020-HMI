#ifndef GUNMODEBARRELREQUEST_H
#define GUNMODEBARRELREQUEST_H

#include "src/infra/core/base_request.h"

class GunModeBarrelRequest: public BaseModel
{
public:
    GunModeBarrelRequest();
    GunModeBarrelRequest(bool manualMode);

    bool getManualMode() const;
    void setManualMode(bool newManualMode);

    // BaseModel interface
    QByteArray toJSON() override;

private:
    bool manualMode;

};
#endif // GUNMODEBARRELREQUEST_H
