#include "gun_mode_barrel_request.h"
#include "qjsondocument.h"
#include "qjsonobject.h"

GunModeBarrelRequest::GunModeBarrelRequest()
{

}

GunModeBarrelRequest::GunModeBarrelRequest(bool manualMode) : manualMode(manualMode)
{}

bool GunModeBarrelRequest::getManualMode() const
{
    return manualMode;
}

void GunModeBarrelRequest::setManualMode(bool newManualMode)
{
    manualMode = newManualMode;
}

QByteArray GunModeBarrelRequest::toJSON()
{
    QJsonObject obj;
    obj["manual_mode"] = manualMode;

    return QJsonDocument(obj).toJson();
}
