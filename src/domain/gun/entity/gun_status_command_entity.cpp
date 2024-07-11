#include "gun_status_command_entity.h"

GunStatusCommandEntity::GunStatusCommandEntity(bool mount, bool single_shot, bool fire, bool proximity, bool siren) : _mount(mount),
    _single_shot(single_shot),
    _fire(fire),
    _proximity(proximity),
    _siren(siren)
{}

bool GunStatusCommandEntity::mount() const
{
    return _mount;
}

void GunStatusCommandEntity::setMount(bool newMount)
{
    _mount = newMount;
}

bool GunStatusCommandEntity::single_shot() const
{
    return _single_shot;
}

void GunStatusCommandEntity::setSingle_shot(bool newSingle_shot)
{
    _single_shot = newSingle_shot;
}

bool GunStatusCommandEntity::fire() const
{
    return _fire;
}

void GunStatusCommandEntity::setFire(bool newFire)
{
    _fire = newFire;
}

bool GunStatusCommandEntity::proximity() const
{
    return _proximity;
}

void GunStatusCommandEntity::setProximity(bool newProximity)
{
    _proximity = newProximity;
}

bool GunStatusCommandEntity::siren() const
{
    return _siren;
}

void GunStatusCommandEntity::setSiren(bool newSiren)
{
    _siren = newSiren;
}
