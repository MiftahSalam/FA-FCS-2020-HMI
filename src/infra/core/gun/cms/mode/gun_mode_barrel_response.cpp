#include "gun_mode_barrel_response.h"

GunModeBarrelResponse::GunModeBarrelResponse()
{

}

GunModeBarrelResponse::GunModeBarrelResponse(bool manualMode) : manualMode(manualMode)
{}

bool GunModeBarrelResponse::getManualMode() const
{
    return manualMode;
}

GunModeBarrelResponse GunModeBarrelResponse::FromJsonObject(QJsonObject obj)
{
    GunModeBarrelResponse model(
                obj.value("manual_mode").toBool()
                );

    return model;
}
