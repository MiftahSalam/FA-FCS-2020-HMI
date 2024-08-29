#include "gun_command_status_request.h"
#include "qjsondocument.h"
#include "qjsonobject.h"

GunCommandStatusRequest::GunCommandStatusRequest()
{

}

GunCommandStatusRequest::GunCommandStatusRequest(bool mount, bool singleShot, bool fireOrder, bool proxFuze, bool siren) : mount(mount),
    singleShot(singleShot),
    fireOrder(fireOrder),
    proxFuze(proxFuze),
    siren(siren)
{}

bool GunCommandStatusRequest::getMount() const
{
    return mount;
}

void GunCommandStatusRequest::setMount(bool newMount)
{
    mount = newMount;
}

bool GunCommandStatusRequest::getSingleShot() const
{
    return singleShot;
}

void GunCommandStatusRequest::setSingleShot(bool newSingleShot)
{
    singleShot = newSingleShot;
}

bool GunCommandStatusRequest::getFireOrder() const
{
    return fireOrder;
}

void GunCommandStatusRequest::setFireOrder(bool newFireOrder)
{
    fireOrder = newFireOrder;
}

bool GunCommandStatusRequest::getProxFuze() const
{
    return proxFuze;
}

void GunCommandStatusRequest::setProxFuze(bool newProxFuze)
{
    proxFuze = newProxFuze;
}

bool GunCommandStatusRequest::getSiren() const
{
    return siren;
}

void GunCommandStatusRequest::setSiren(bool newSiren)
{
    siren = newSiren;
}

QByteArray GunCommandStatusRequest::toJSON()
{
    QJsonObject obj;
    obj["mount"] = mount;
    obj["single_shot"] = singleShot;
    obj["fire_order"] = fireOrder;
    obj["not_fire_order"] = !fireOrder;
    obj["paralizing_proximity_fuze"] = proxFuze;
    obj["siren_button"] = siren;

    return QJsonDocument(obj).toJson();
}
