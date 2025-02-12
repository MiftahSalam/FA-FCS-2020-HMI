#include "gun_command_status_response.h"

GunCommandStatusResponse::GunCommandStatusResponse()
{

}

GunCommandStatusResponse::GunCommandStatusResponse(bool mount, bool singleShot, bool fireOrder, bool notFireOrder, bool proxFuze, bool siren) : mount(mount),
    singleShot(singleShot),
    fireOrder(fireOrder),
    notFireOrder(notFireOrder),
    proxFuze(proxFuze),
    siren(siren)
{}

GunCommandStatusResponse GunCommandStatusResponse::FromJsonObject(QJsonObject obj)
{
    GunCommandStatusResponse model(
        obj.value("mount").toBool(),
        obj.value("single_shot").toBool(),
        obj.value("fire_order").toBool(),
        obj.value("not_fire_order").toBool(),
        obj.value("paralizing_proximity_fuze").toBool(),
        obj.value("siren_button").toBool()
        );

    return model;
}

bool GunCommandStatusResponse::getMount() const
{
    return mount;
}

bool GunCommandStatusResponse::getSingleShot() const
{
    return singleShot;
}

bool GunCommandStatusResponse::getFireOrder() const
{
    return fireOrder;
}

bool GunCommandStatusResponse::getProxFuze() const
{
    return proxFuze;
}

bool GunCommandStatusResponse::getSiren() const
{
    return siren;
}
