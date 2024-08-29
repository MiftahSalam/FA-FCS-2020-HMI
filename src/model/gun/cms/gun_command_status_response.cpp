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
